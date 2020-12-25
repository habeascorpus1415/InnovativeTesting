// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BasePlayerState.h"

#include "BaseCharacter.generated.h"

UCLASS()
class INNOVATIVETESTING_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

	/* private: UPROPERTY LIST */

public:

	/* public: UPROPERTY LIST */

private:

	/* private: UFUNCTION LIST */

public:

	/* public: UFUNCTION LIST */

	/**
	* Getters for attributes from UBaseAttributeSet
	**/

	UFUNCTION(BlueprintCallable, Category = Attributes)
	float GetHealth() const;

private:

	/* private: VARIABLE LIST */

public:

	/* public: VARIABLE LIST */

protected:

	/* protected: VARIABLE LIST */

private:

	/* private: FUNCTION LIST */

public:

	/* public: FUNCTION LIST */

	// Sets default values for this character's properties
	ABaseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/* protected: FUNCTION LIST */

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	/* Initialize Attributes */
	virtual void InitializeAttributes();

	/* Base Effect */
	virtual void AddStartupEffects();

	/* Base Abilities */
	virtual void AddCharacterAbilities();

};
