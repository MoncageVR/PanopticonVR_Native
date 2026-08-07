

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CoreCommon/Enum/EPrisonerStates.h"
#include "FPrisonerInfoRow.generated.h"

//enum class EPrisonerUpperState : uint8;
//enum class EPrisonerLowerState : uint8;

USTRUCT(BlueprintType)
struct FPrisonerInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EPrisonerUpperStateType>> ActionUpperState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EPrisonerLowerStateType>> ActionLowerState;

};
