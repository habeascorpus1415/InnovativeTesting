// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class INNOVATIVETESTING_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UBaseAbilitySystemComponent();

	// ??????? 
	bool CharacterAbilitiesGiven = false;

	// ??????? 
	bool StartupEffectsApplied = false;

};
