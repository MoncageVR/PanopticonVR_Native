// Fill out your copyright notice in the Description page of Project Settings.

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

	void GameOverCheckEvent();

#pragma region Setter
	FORCEINLINE void SetIsGameOverFlag(bool bInFlag) { bIsGameOverFlag = bInFlag; }
#pragma endregion

#pragma region Getter
	FORCEINLINE bool GetIsGameOverFlag() const { return bIsGameOverFlag; }
#pragma endregion


protected:
	//void InitPrisonerBBVariables(class APrisonerController* InTempContorller, int32 InIndex);

	UPROPERTY()
	TObjectPtr<class ACVRPawn> MyVRPawn;

private:
	class UVRGameInstance* MyVRGameInstance;

	FTimerHandle Debug_Delay_Timer;

	bool bIsGameOverFlag;



private:
};
