// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"

#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"


UBaseAttributeSet::UBaseAttributeSet()
{
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/**
	* REPTNOTIFY_Always tells the OnRep function to trigger if the local value is already equal to the value being repped down from the Server (due to prediction).
	* By default it won't trigger the OnRep function if the local value is the same as the value being repped down from the Server.
	*/

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Stamina, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);

}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent * Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer & SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	AActor * TargetActor = nullptr;
	AController * TargetController = nullptr;
	ACharacter * TargetCharacter = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ACharacter>(TargetActor);
	}


	// Get the Source actor
	AActor * SourceActor = nullptr;
	AController * SourceController = nullptr;
	ACharacter * SourceCharacter = nullptr;

	if (IsValid(Source) && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (IsValid(SourceController))
		{
			SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<ACharacter>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}


	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		/** Needed added maxhealth value */
		const float MaxHealth = 100.f;

		// Apply the health change and then clamp it
		const float NewHealth = GetHealth();

		// If character was alive before damage is added, handle damage
		// This prevents damage being added to dead things and replaying death animations
		bool WasAlive = true;

		if (NewHealth > 0.f)
		{
			WasAlive = true;
		}
		else
		{
			WasAlive = false;
		}

		SetHealth(FMath::Clamp(NewHealth, 0.0f, MaxHealth));

	}//Health
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		/** Needed added maxhealth value */
		const float MaxStamina = 100.f;

		SetStamina(FMath::Clamp(GetStamina(), 0.f, MaxStamina));
	}//Stamina
	else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{

	}//MoveSpeed
}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute & Attribute, float & NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

}

/** GAMEPLAYATTRIBUTE_REPNOTIFY() This is a helper macro that can be used in RepNotify functions to handle attributes that will be predictively modified by clients. */
void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldHealth);
}

/** GAMEPLAYATTRIBUTE_REPNOTIFY() This is a helper macro that can be used in RepNotify functions to handle attributes that will be predictively modified by clients. */
void UBaseAttributeSet::OnRep_Stamina(const FGameplayAttributeData & OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Stamina, OldStamina);
}

/** GAMEPLAYATTRIBUTE_REPNOTIFY() This is a helper macro that can be used in RepNotify functions to handle attributes that will be predictively modified by clients. */
void UBaseAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData & OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MoveSpeed, OldMoveSpeed);
}
