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
