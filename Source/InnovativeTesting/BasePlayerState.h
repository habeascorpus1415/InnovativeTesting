// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "BaseAbilitySystemComponent.h"
#include "BaseAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "Ability/BaseGameplayAbility.h"

#include "BasePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class INNOVATIVETESTING_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:

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

public:

	/* public: UFUNCTION LIST */

protected:

	/* protected: UFUNCTION LIST */

private:

	/* private: VARIABLE LIST */

public:

	/* public: VARIABLE LIST */

protected:

	/* protected: VARIABLE LIST */

	FDelegateHandle HealthChangedDelegateHandle;

private:

	/* private: FUNCTION LIST */

public:

	/* public: FUNCTION LIST */

	ABasePlayerState();

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent * GetAbilitySystemComponent() const override;

	// GetAttribute
	virtual class UBaseAttributeSet * GetAttributeSetBase() const;

protected:

	/* protected: FUNCTION LIST */

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
};
