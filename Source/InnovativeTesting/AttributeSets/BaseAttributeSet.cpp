// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"


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
