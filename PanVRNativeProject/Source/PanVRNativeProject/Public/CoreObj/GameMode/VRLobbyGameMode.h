#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VRLobbyGameMode.generated.h"

DECLARE_DELEGATE_OneParam(FOnGameResultPrintDelegateVar, bool);

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AVRLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVRLobbyGameMode();

	virtual void StartPlay() override;

	UFUNCTION()
	void HandleOpenMainMap();
	UFUNCTION()
	void CheckGameResult();

#pragma region DelegateRelated
	FOnGameResultPrintDelegateVar FGameResultPrintSignature;

	UFUNCTION()
	void NotifyGameResultPrint(bool bIsGameResultFlag);
#pragma endregion

#pragma region Getter
	TArray<TObjectPtr<class AATape>> const GetTapeArrs() { return TapeArrs; }
#pragma endregion

#pragma region Setter

#pragma endregion

protected:
	UPROPERTY()
	TObjectPtr<class UVRGameInstance> mVRGameInstanceRef;


private:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class AATape>> TapeArrs;
};
