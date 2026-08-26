#pragma once

#include "CoreMinimal.h"
#include "CoreObj/Manager/GameInstanceSubSystem/VRGameInstanceSubsystem.h"
#include "VRDialogueManagerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDialogueChangeSignature, const FText&, Text, float, PrintTime, float, TotalPrintTime, int32, SoundNum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueLineFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialoguePauseSignature, bool, bPause);

class UUserWidget;

UCLASS()
class PANVRNATIVEPROJECT_API UVRDialogueManagerSubsystem : public UVRGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnDialogueChangeSignature FDialogueChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDialogueLineFinished OnDialogueLineFinished;

	UPROPERTY(BlueprintAssignable)
	FOnDialoguePauseSignature OnDialoguePauseToggle;

	UFUNCTION()
	void StartDialogue();

	UFUNCTION(BlueprintCallable)
	void PlayDialogue(class UDataTable* InDT);

	UFUNCTION(BlueprintCallable)
	void NotifyLineFinished();

	UFUNCTION(BlueprintCallable)
	void PauseDialogue();

	UFUNCTION(BlueprintCallable)
	void ResumeDialogue();

#pragma region Getter
	FORCEINLINE int32 GetCurrIndex() const { return CurrentIndex; }
#pragma endregion

private:
	UPROPERTY()
	TObjectPtr<class UDataTable> DTDialogueTuto1;

	UPROPERTY()
	TObjectPtr<class UDataTable> DTDialogueTuto2;

	UPROPERTY()
	TObjectPtr<class UDataTable> DTDialogueTuto3;

	UPROPERTY()
	TObjectPtr<UDataTable> CurrentDT;

	TArray<FName> RowNames;
	int32 CurrentIndex = 0;
	bool bIsPaused = false;
	bool bPendingNextLine = false;

private:
	void PlayCurrentLine();
};
