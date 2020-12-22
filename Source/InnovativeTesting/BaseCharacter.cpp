// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

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

	L_PlayerState->GetAttributeSetBase()->SetHealth(100.f);

	InitializeAttributes();
}

void ABaseCharacter::InitializeAttributes()
{
	ABasePlayerState * L_PlayerState = GetPlayerState<ABasePlayerState>();

	if (IsValid(L_PlayerState))
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
