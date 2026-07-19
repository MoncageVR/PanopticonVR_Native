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

protected:
	//void InitPrisonerBBVariables(class APrisonerController* InTempContorller, int32 InIndex);

private:
	class UVRGameInstance* MyVRGameInstance;

	FTimerHandle Debug_Delay_Timer;

private:
	void Debug_StartPlay_Delay_Func(); // Debug
};
