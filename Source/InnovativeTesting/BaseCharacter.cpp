// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Ability/BaseGameplayAbility.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	ABasePlayerState * L_PlayerState = GetPlayerState<ABasePlayerState>();

	if (!IsValid(L_PlayerState))
	{
		return;
	}

	L_PlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(L_PlayerState, this);

	if (!IsValid(L_PlayerState->GetAttributeSetBase()))
	{
		return;
	}

	//L_PlayerState->GetAttributeSetBase()->SetHealth(100.f);

	InitializeAttributes();

	AddStartupEffects();

	AddCharacterAbilities();
}

void ABaseCharacter::InitializeAttributes()
{
	ABasePlayerState * L_PlayerState = GetPlayerState<ABasePlayerState>();

	if (!IsValid(L_PlayerState))
	{
		return;
	}

	if (!IsValid(L_PlayerState->GetAbilitySystemComponent()))
	{
		return;
	}

	if (!IsValid(L_PlayerState->DefaultAttributes))
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = L_PlayerState->GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = L_PlayerState->GetAbilitySystemComponent()->
		MakeOutgoingSpec(L_PlayerState->DefaultAttributes, 1.0f, EffectContext);

	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = L_PlayerState->GetAbilitySystemComponent()->
			ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), L_PlayerState->GetAbilitySystemComponent());
	}

}

void ABaseCharacter::AddStartupEffects()
{
	ABasePlayerState * L_PlayerState = GetPlayerState<ABasePlayerState>();

	if (!IsValid(L_PlayerState))
	{
		return;
	}

	if (!IsValid(L_PlayerState->GetAbilitySystemComponent()))
	{
		return;
	}

	UBaseAbilitySystemComponent * L_AbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(L_PlayerState->GetAbilitySystemComponent());

	if (!IsValid(L_AbilitySystemComponent))
	{
		return;
	}

	if (GetLocalRole() != ROLE_Authority ||	L_AbilitySystemComponent->StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = L_AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : L_PlayerState->StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = L_AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1.0f, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = L_AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), L_AbilitySystemComponent);
		}
	}

	L_AbilitySystemComponent->StartupEffectsApplied = true;
}

void ABaseCharacter::AddCharacterAbilities()
{
	/*if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	ABasePlayerState * L_PlayerState = GetPlayerState<ABasePlayerState>();

	if (!IsValid(L_PlayerState))
	{
		return;
	}

	UBaseAbilitySystemComponent * L_AbilitySystemComponent = CastChecked<UBaseAbilitySystemComponent>(L_PlayerState->GetAbilitySystemComponent());

	if (!IsValid(L_AbilitySystemComponent))
	{
		return;
	}

	if (L_AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UBaseGameplayAbility>& StartupAbility : L_PlayerState->StartupAbilities)
	{
		L_AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1.0f, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	L_AbilitySystemComponent->CharacterAbilitiesGiven = true;*/
}

float ABaseCharacter::GetHealth() const
{
	ABasePlayerState * L_PlayerState = GetPlayerState<ABasePlayerState>();

	if (!IsValid(L_PlayerState))
	{
		return 0.0f;
	}

	if (!IsValid(L_PlayerState->GetAttributeSetBase()))
	{
		return 0.0f;
	}

	return L_PlayerState->GetAttributeSetBase()->GetHealth();
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

}
