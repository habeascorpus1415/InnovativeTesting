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

	//The Flags parameter contains the compressed input flags that are stored in the saved move.
	//UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	//It basically just resets the movement component to the state when the move was made so it can simulate from there.
	RequestToStartSprinting = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;

	RequestToStartADS = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;

}

FNetworkPredictionData_Client * UBaseCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);

	if (!ClientPredictionData)
	{
		UBaseCharacterMovementComponent * MutableThis = const_cast<UBaseCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FBaseNetworkPredictionData_Client(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UBaseCharacterMovementComponent::StopSprinting()
{
	SprintSpeedMultiplier = 1.f;
}

void UBaseCharacterMovementComponent::StartSprinting(float NewSprintSpeedMultiplier)
{
	SprintSpeedMultiplier = NewSprintSpeedMultiplier;
}

void UBaseCharacterMovementComponent::FBaseSavedMove::Clear()
{
	Super::Clear();

	SavedRequestToStartSprinting = false;
	SavedRequestToStartADS = false;
}

uint8 UBaseCharacterMovementComponent::FBaseSavedMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedRequestToStartSprinting)
	{
		Result |= FLAG_Custom_0;
	}

	if (SavedRequestToStartADS)
	{
		Result |= FLAG_Custom_1;
	}

	return Result;
}

bool UBaseCharacterMovementComponent::FBaseSavedMove::CanCombineWith(const FSavedMovePtr & NewMove, ACharacter * Character, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (SavedRequestToStartSprinting != ((FBaseSavedMove*)&NewMove)->SavedRequestToStartSprinting)
	{
		return false;
	}

	if (SavedRequestToStartADS != ((FBaseSavedMove*)&NewMove)->SavedRequestToStartADS)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UBaseCharacterMovementComponent::FBaseSavedMove::SetMoveFor(ACharacter * Character, float InDeltaTime, FVector const & NewAccel, FNetworkPredictionData_Client_Character & ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UBaseCharacterMovementComponent* CharacterMovement = Cast<UBaseCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		SavedRequestToStartSprinting = CharacterMovement->RequestToStartSprinting;
		SavedRequestToStartADS = CharacterMovement->RequestToStartADS;
	}
}

void UBaseCharacterMovementComponent::FBaseSavedMove::PrepMoveFor(ACharacter * Character)
{
	Super::PrepMoveFor(Character);

	UBaseCharacterMovementComponent* CharacterMovement = Cast<UBaseCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
	}
}

UBaseCharacterMovementComponent::FBaseNetworkPredictionData_Client::FBaseNetworkPredictionData_Client(const UCharacterMovementComponent & ClientMovement) : Super(ClientMovement)
{
	//
}

FSavedMovePtr UBaseCharacterMovementComponent::FBaseNetworkPredictionData_Client::AllocateNewMove()
{
	return FSavedMovePtr(new FBaseSavedMove());
}
