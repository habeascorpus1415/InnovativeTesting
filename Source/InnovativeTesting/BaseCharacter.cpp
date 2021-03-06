// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "MovementComponent/BaseCharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

#define LEVELCHARACTER 1.0f

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, AbilitySystemComponent); 
	DOREPLIFETIME(ABaseCharacter, DefaultAttributes);
	DOREPLIFETIME(ABaseCharacter, StartupEffects);
	DOREPLIFETIME(ABaseCharacter, StartupAbilities);

}

ABaseCharacter::ABaseCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;

	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	AttributeSetBase = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSetBase"));

	/** Attribute change callbacks */
	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ABaseCharacter::HealthChanged);

	StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaAttribute()).AddUObject(this, &ABaseCharacter::StaminaChanged);

}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);

	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	// ASC MixedMode replication requires that the ASC Owner's Owner be the Controller.
	SetOwner(NewController);

	check(AbilitySystemComponent);

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	if (!IsValid(GetAttributeSetBase()))
	{
		return;
	}

	InitializeAttributes();

	AddStartupEffects();

	AddCharacterAbilities();
}

void ABaseCharacter::InitializeAttributes()
{
	check(DefaultAttributes);

	if (!IsValid(DefaultAttributes))
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->
		MakeOutgoingSpec(DefaultAttributes, LEVELCHARACTER, EffectContext);

	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->
			ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
	}

}

void ABaseCharacter::AddStartupEffects()
{
	check(AbilitySystemComponent);

	if (GetLocalRole() != ROLE_Authority || AbilitySystemComponent->StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, LEVELCHARACTER, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->
				ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}

	AbilitySystemComponent->StartupEffectsApplied = true;
}

void ABaseCharacter::AddCharacterAbilities()
{
	check(AbilitySystemComponent);

	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UBaseGameplayAbility>& StartupAbility : StartupAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1.0f, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

float ABaseCharacter::GetHealth() const
{
	check(AttributeSetBase);

	return GetAttributeSetBase()->GetHealth();
}

float ABaseCharacter::GetStamina() const
{
	check(AttributeSetBase);

	return GetAttributeSetBase()->GetStamina();
}

float ABaseCharacter::GetMoveSpeed() const
{
	check(AttributeSetBase);

	return GetAttributeSetBase()->GetMoveSpeed();
}

float ABaseCharacter::GetMoveSpeedBaseValue() const
{
	check(AttributeSetBase);

	return GetAttributeSetBase()->GetMoveSpeedAttribute().GetGameplayAttributeData(GetAttributeSetBase())->GetBaseValue();
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** Binding input for ASC */
	BindASCInput();
}

UAbilitySystemComponent * ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UBaseAttributeSet * ABaseCharacter::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

void ABaseCharacter::RemoveCharacterAbilities()
{
	check(AbilitySystemComponent);

	if (GetLocalRole() != ROLE_Authority || AbilitySystemComponent->StartupEffectsApplied)
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && StartupAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = false;
}

void ABaseCharacter::HealthChanged(const FOnAttributeChangeData & Data)
{
	const float NewHealth = Data.NewValue;

	// Cache tags
	const FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Dead"));

	// If the player died, handle death
	if (NewHealth <= 0 && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		Die();
	}
}

void ABaseCharacter::StaminaChanged(const FOnAttributeChangeData & Data)
{
	float Stamina = Data.NewValue;
}

void ABaseCharacter::Die()
{
	// Only runs on Server
	RemoveCharacterAbilities();

	const FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Dead"));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->CancelAllAbilities();

		//AbilitySystemComponent->RemoveActiveEffects();

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}
}

void ABaseCharacter::BindASCInput()
{
	check(InputComponent);

	check(AbilitySystemComponent);

	if (!ASCInputBound && IsValid(InputComponent) && IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(
			InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
				FString("CancelTarget"), FString("EBaseAbilityInputID"),
				static_cast<int32>(EBaseAbilityInputID::Confirm), static_cast<int32>(EBaseAbilityInputID::Cancel))
		);

		ASCInputBound = true;
	}
}