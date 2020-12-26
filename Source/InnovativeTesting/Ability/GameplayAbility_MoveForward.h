// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "GameplayAbility_MoveForward.generated.h"

/**
 * 
 */
UCLASS()
class INNOVATIVETESTING_API UGameplayAbility_MoveForward : public UBaseGameplayAbility
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

protected:

	/* protected: UFUNCTION LIST */

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

	UGameplayAbility_MoveForward();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

protected:

	/* protected: FUNCTION LIST */
};
