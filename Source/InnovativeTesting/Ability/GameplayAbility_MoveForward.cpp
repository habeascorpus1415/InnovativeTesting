// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbility_MoveForward.h"

#define VALUEMOVEFOWARD 1

UGameplayAbility_MoveForward::UGameplayAbility_MoveForward()
{
	AbilityInputID = EBaseAbilityInputID::MoveForward;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("locomotion.MoveForward")));
}

bool UGameplayAbility_MoveForward::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayTagContainer * SourceTags, const FGameplayTagContainer * TargetTags, OUT FGameplayTagContainer * OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const APawn * Pawn = CastChecked<APawn>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);

	if ((Pawn->Controller != nullptr) && (VALUEMOVEFOWARD != 0.0f))
	{
		return true;
	}

	return false;
}

void UGameplayAbility_MoveForward::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData * TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
	
	APawn * Pawn = CastChecked<APawn>(OwnerInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);

	// find out which way is forward
	const FRotator Rotation = Pawn->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	Pawn->AddMovementInput(Direction, VALUEMOVEFOWARD);
}

void UGameplayAbility_MoveForward::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

}

void UGameplayAbility_MoveForward::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}
