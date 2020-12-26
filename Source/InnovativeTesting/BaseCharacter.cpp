// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Net/UnrealNetwork.h"

#define LEVELCHARACTER 1.0f

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	AttributeSetBase = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSetBase"));

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

	if (AbilitySystemComponent->CharacterAbilitiesGiven)
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
	return GetAttributeSetBase()->GetHealth();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(InputComponent);

	check(AbilitySystemComponent);

	if (!ASCInputBound)
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(
			InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
				FString("CancelTarget"), FString("EBaseAbilityInputID"),
				static_cast<int32>(EBaseAbilityInputID::Confirm), static_cast<int32>(EBaseAbilityInputID::Cancel))
		);

		ASCInputBound = true;
	}
}

UAbilitySystemComponent * ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UBaseAttributeSet * ABaseCharacter::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

void ABaseCharacter::OnRep_PlayerState()
{

}

void ABaseCharacter::HealthChanged(const FOnAttributeChangeData & Data)
{
}

