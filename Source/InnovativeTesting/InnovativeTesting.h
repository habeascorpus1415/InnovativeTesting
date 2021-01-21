// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class EBaseAbilityInputID : uint8
{
	// 0 None
	None				UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm				UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel				UMETA(DisplayName = "Cancel"),
	// 3 MoveForward
	MoveForward			UMETA(DisplayName = "MoveForward"),
	// 4 MoveRight
	MoveRight			UMETA(DisplayName = "MoveRight"),
	// 5 Sprint
	Sprint				UMETA(DisplayName = "Sprint"),
	// 6 Jump
	Jump				UMETA(DisplayName = "Jump"),
	// 7 PrimaryAction
	PrimaryAction		UMETA(DisplayName = "PrimaryAction"),
	// 8 SecondaryAction
	SecondaryAction		UMETA(DisplayName = "SecondaryAction")

};
