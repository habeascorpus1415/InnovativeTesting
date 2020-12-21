// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class INNOVATIVETESTING_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

private:

	/* private: UPROPERTY LIST */

public:

	/* public: UPROPERTY LIST */

	/**
	*	Tip: If you don't want an Attribute to show up in the Editor's list of Attributes, 
	*	you can use the Meta = (HideInDetailsView) property specifier.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health)

protected:

	/* protected: UPROPERTY LIST */

private:

	/* private: UFUNCTION LIST */

public:

	/* public: UFUNCTION LIST */

protected:

	/* protected: UFUNCTION LIST */

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

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

	UBaseAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 *	Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made.
	 *	Note this is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute. It is not called during an application of a GameplayEffect, such as a 5 ssecond +10 movement speed buff.
	 */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/**
	 *	Called just before any modification happens to an attribute. This is lower level than PreAttributeModify/PostAttribute modify.
	 *	There is no additional context provided here since anything can trigger this. Executed effects, duration based effects, effects being removed, immunity being applied, stacking rules changing, etc.
	 *	This function is meant to enforce things like "Health = Clamp(Health, 0, MaxHealth)" and NOT things like "trigger this extra thing if damage is applied, etc".
	 *
	 *	NewValue is a mutable reference so you are able to clamp the newly applied value as well.
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

protected:

	/* protected: FUNCTION LIST */

};
