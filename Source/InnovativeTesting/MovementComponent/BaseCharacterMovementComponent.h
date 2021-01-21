// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class INNOVATIVETESTING_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()


	class FBaseSavedMove : public FSavedMove_Character
	{
	public:

		typedef FSavedMove_Character Super;

		///@brief Resets all saved variables.
		virtual void Clear() override;

		///@brief Store input commands in the compressed flags.
		virtual uint8 GetCompressedFlags() const override;

		///@brief This is used to check whether or not two moves can be combined into one.
		///Basically you just check to make sure that the saved variables are the same.
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;

		///@brief Sets up the move before sending it to the server. 
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData) override;
		///@brief Sets variables on character movement component before making a predictive correction.
		virtual void PrepMoveFor(class ACharacter* Character) override;

		// Sprint
		uint8 SavedRequestToStartSprinting : 1;

		// Aim Down Sights
		uint8 SavedRequestToStartADS : 1;
	};

	class FBaseNetworkPredictionData_Client : public FNetworkPredictionData_Client_Character
	{
	public:
		FBaseNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		///@brief Allocates a new copy of our custom saved move
		virtual FSavedMovePtr AllocateNewMove() override;
	};

private:

	/* private: UPROPERTY LIST */

public:

	/* public: UPROPERTY LIST */

protected:

	/* protected: UPROPERTY LIST */

private:

	/* private: UFUNCTION LIST */

public:

	/* public: UFUNCTION LIST */

		// Sprint
	UFUNCTION(BlueprintCallable)
	virtual void StartSprinting(float NewSprintSpeedMultiplier);

	UFUNCTION(BlueprintCallable)
	virtual void StopSprinting();

protected:

	/* protected: UFUNCTION LIST */

private:

	/* private: VARIABLE LIST */

	float SprintSpeedMultiplier = 1.f;

public:

	/* public: VARIABLE LIST */

	uint8 RequestToStartSprinting : 1;

	uint8 RequestToStartADS : 1;

protected:

	/* protected: VARIABLE LIST */

private:

	/* private: FUNCTION LIST */

public:

	/* public: FUNCTION LIST */

	virtual float GetMaxSpeed() const override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:

	/* protected: FUNCTION LIST */

};
