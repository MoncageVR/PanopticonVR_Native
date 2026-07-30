// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VRLobbyGameMode.generated.h"

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
protected:
	UPROPERTY()
	TObjectPtr<class UVRGameInstance> mVRGameInstanceRef;

private:
	
};
