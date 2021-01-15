// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterMovementComponent.h"
#include "../BaseCharacter.h"

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	ABaseCharacter * Owner = Cast<ABaseCharacter>(GetOwner());
	if (!IsValid(Owner))
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));

		return Super::GetMaxSpeed();
	}

	return Owner->GetMoveSpeed() * SprintSpeedMultiplier;
}

void UBaseCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

}

FNetworkPredictionData_Client * UBaseCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);

	return nullptr;
}

void UBaseCharacterMovementComponent::StopSprinting()
{
	SprintSpeedMultiplier = 1.f;
}

void UBaseCharacterMovementComponent::StartSprinting(float NewSprintSpeedMultiplier)
{
	SprintSpeedMultiplier = NewSprintSpeedMultiplier;
}
