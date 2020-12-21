// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
//#include "BaseAttributeSet"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ABaseCharacter::HealthChanged);
}

void ABaseCharacter::HealthChanged(const FOnAttributeChangeData & Data)
{
	UE_LOG(LogTemp, Display, TEXT("ABaseCharacter::HealthChanged: NewValue = %f, OldValue = %f, "), Data.NewValue, Data.OldValue);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent * ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

float ABaseCharacter::GetHealth() const
{
	if (!AttributeSetBase.IsValid())
	{
		return 0.0f;
	}

	return AttributeSetBase->GetHealth();
}