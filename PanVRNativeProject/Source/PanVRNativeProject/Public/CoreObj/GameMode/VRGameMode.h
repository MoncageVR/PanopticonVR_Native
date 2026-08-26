#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VRGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AVRGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AVRGameMode();

	virtual void StartPlay() override;
	virtual void InitGameState() override;

	UFUNCTION()
	void GameOverCheckEvent();

	UFUNCTION(BlueprintCallable)
	void GameSuccessCheckEvent();

	void ClearGameResultFlag();

	// T : Add Prisoner , F : Remove Prisoner - Prisoner State Is Only Teleport And Floating
	void HandleListOfFloatNTelePrisoners(uint8 InHandleFlag, int32 InUniqueNum);

	// T : Add Prisoner , F : RemovePrisoner - Prisoner State Is Only Flame
	void HandleListOfFlamePrisoners(uint8 InHandleFlag, int32 InFlameUniqueNum);

#pragma region Setter
	FORCEINLINE void SetIsGameOverFlag(bool bInFlag) { bIsGameOverFlag = bInFlag; }
	UFUNCTION(BlueprintCallable)
	void SetIsGameSuccessFlag(bool bResultFlag) { bIsGameSuccessFlag = bResultFlag; }
#pragma endregion

#pragma region Getter
	FORCEINLINE bool GetIsGameOverFlag() const { return bIsGameOverFlag; }
	FORCEINLINE bool GetIsGameSuccessFlag() const { return bIsGameSuccessFlag; }
	FORCEINLINE TArray<int32> GetListFloatNTelePrisoner() const { return ListOfPrisonersWithFloatingORTeleportStatus; }
	FORCEINLINE TArray<int32> GetListFlamePrisoners() const { return ListOfPrisonersWithFlameStatus; }
#pragma endregion


protected:
	//void InitPrisonerBBVariables(class APrisonerController* InTempContorller, int32 InIndex);

	UPROPERTY()
	TObjectPtr<class ACVRPawn> MyVRPawn;

private:
	UPROPERTY()
	class UVRGameInstance* MyVRGameInstance;

	FTimerHandle Debug_Delay_Timer;

	bool bIsGameOverFlag;
	bool bIsGameSuccessFlag;

	UPROPERTY(VisibleAnywhere)
	TArray<int32> ListOfPrisonersWithFloatingORTeleportStatus;

	UPROPERTY(VisibleAnywhere)
	TArray<int32> ListOfPrisonersWithFlameStatus;

	UPROPERTY()
	TObjectPtr<class UPrisonerManagerSubsystem> UPrisonerMgrSubSyPtr;

	UPROPERTY()
	TObjectPtr<class UMapObjManagerSubsystem> UMapObjMgrSubSyPtr;

	UPROPERTY()
	TObjectPtr<class UVREquipmentWorldSubsystem> VREquipWorldSubSyPtr;

private:
	UFUNCTION()
	void HandleGMReceiveByGTW(bool bIsGameStartFlag);
};
