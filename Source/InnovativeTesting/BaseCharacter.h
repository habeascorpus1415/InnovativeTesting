// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BaseAbilitySystemComponent.h"
#include "AttributeSets/BaseAttributeSet.h"
#include "Ability/BaseGameplayAbility.h"

#include "AbilitySystemInterface.h"

#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ABaseCharacter*, Character);

UCLASS()
class INNOVATIVETESTING_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/* private: UPROPERTY LIST */

public:

	/* public: UPROPERTY LIST */

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UBaseGameplayAbility>> StartupAbilities;

	UPROPERTY(BlueprintAssignable, Category = GAS)
	FCharacterDiedDelegate OnCharacterDied;

protected:

	/* protected: UPROPERTY LIST */

	/* Ability component */
	UPROPERTY()
	class UBaseAbilitySystemComponent * AbilitySystemComponent;

	/* Ability attribute */
	UPROPERTY()
	class UBaseAttributeSet * AttributeSetBase;

private:

	/* private: UFUNCTION LIST */

	virtual void Die();

public:

	/* public: UFUNCTION LIST */

	/**
	* Getters for attributes from UBaseAttributeSet
	**/

	UFUNCTION(BlueprintCallable, Category = Attributes)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = Attributes)
	float GetStamina() const;

	/** Gets the Current value of MoveSpeed */
	UFUNCTION(BlueprintCallable, Category = Attributes)
	float GetMoveSpeed() const;

	/** Gets the Base value of MoveSpeed */
	UFUNCTION(BlueprintCallable, Category = Attributes)
	float GetMoveSpeedBaseValue() const;

private:

	/* private: VARIABLE LIST */

public:

	/* public: VARIABLE LIST */

protected:

	/* protected: VARIABLE LIST */
	
	/* Variable for asked bound player with input or not */
	bool ASCInputBound = false;

	/** The delegate for variables when changed */
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle StaminaChangedDelegateHandle;

private:

	/* private: FUNCTION LIST */

public:

	/* public: FUNCTION LIST */

	// Sets default values for this character's properties
	ABaseCharacter(const class FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Implement IAbilitySystemInterface */
	virtual class UAbilitySystemComponent * GetAbilitySystemComponent() const override;

	/** GetAttribute */
	virtual class UBaseAttributeSet * GetAttributeSetBase() const;

	/** Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too. */
	virtual void RemoveCharacterAbilities();

protected:

	/* protected: FUNCTION LIST */

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

	/* Initialize Attributes */
	virtual void InitializeAttributes();

	/* Base Effect */
	virtual void AddStartupEffects();

	/* Base Abilities */
	virtual void AddCharacterAbilities();

	//** Attribute changed callbacks */
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostInitializeComponents() override;
};
