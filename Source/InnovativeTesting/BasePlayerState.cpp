// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerState.h"


ABasePlayerState::ABasePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSetBase"));

	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent * ABasePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UBaseAttributeSet * ABasePlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

void ABasePlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(GetAbilitySystemComponent()))
	{
		return;
	}

	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ABasePlayerState::HealthChanged);

}

void ABasePlayerState::HealthChanged(const FOnAttributeChangeData & Data)
{
	UE_LOG(LogTemp, Display, TEXT("ABaseCharacter::HealthChanged: NewValue = %f, OldValue = %f, "), Data.NewValue, Data.OldValue);
}
