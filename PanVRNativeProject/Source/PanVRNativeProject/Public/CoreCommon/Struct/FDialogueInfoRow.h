#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FDialogueInfoRow.generated.h"

USTRUCT(BlueprintType)
struct FDialogueInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 num;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText text_en;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float text_time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float text_total_en;
};
