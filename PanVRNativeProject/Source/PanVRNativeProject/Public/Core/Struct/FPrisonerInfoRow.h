// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Core/Enum/EPrisonerStates.h"
#include "FPrisonerInfoRow.generated.h"

//enum class EPrisonerUpperState : uint8;
//enum class EPrisonerLowerState : uint8;

USTRUCT(BlueprintType)
struct FPrisonerInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EPrisonerUpperState>> ActionUpperState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EPrisonerLowerState>> ActionLowerState;

};
