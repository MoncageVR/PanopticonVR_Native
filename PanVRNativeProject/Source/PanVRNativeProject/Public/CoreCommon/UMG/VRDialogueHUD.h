#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VRDialogueHUD.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UVRDialogueHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText TempSaveFullContentText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText CurrContentText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 CurrTextLen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 CurrSoundNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float TextPrintingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float TextPrintingTotalTime;
};
