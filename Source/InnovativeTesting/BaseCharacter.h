// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "UObject/ObjectMacros.h"

#include "BaseAbilitySystemComponent.h"

#include "BaseAttributeSet.h"

#include "BaseCharacter.generated.h"

UCLASS()
class INNOVATIVETESTING_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
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

	/* Ability component */
	TWeakObjectPtr<class UBaseAbilitySystemComponent> AbilitySystemComponent;

	/* Ability attribute */
	TWeakObjectPtr<class UBaseAttributeSet> AttributeSetBase;

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

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	/* protected: FUNCTION LIST */

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Responding to Attribute Changes */
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
};
