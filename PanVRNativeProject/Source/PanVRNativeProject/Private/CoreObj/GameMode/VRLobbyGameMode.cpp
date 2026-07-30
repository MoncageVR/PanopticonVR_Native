// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreObj/GameMode/VRLobbyGameMode.h"
#include "CoreObj/VRGameInstance.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"
#include "VRPawn/CVRPawn.h"
#include "Kismet/GameplayStatics.h"
#include "BPMainActorBase/AGameResultScreen.h"

AVRLobbyGameMode::AVRLobbyGameMode()
{
	DefaultPawnClass = ACVRPawn::StaticClass();
}

void AVRLobbyGameMode::StartPlay()
{
	Super::StartPlay();

	mVRGameInstanceRef = Cast<UVRGameInstance>(GetWorld()->GetGameInstance());
	check(mVRGameInstanceRef);
}

void AVRLobbyGameMode::HandleOpenMainMap()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMap"));
}

void AVRLobbyGameMode::CheckGameResult()
{
	UVREquipmentWorldSubsystem* TempVREquipmentWorldSubSystemRef = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	AAGameResultScreen* TempGameResultScreen = nullptr;

	check(TempVREquipmentWorldSubSystemRef);
	for (IIEquipmentInitInterface* Var : TempVREquipmentWorldSubSystemRef->GetEquipmentArr())
	{
		TempGameResultScreen = Cast<AAGameResultScreen>(Var);
		if (TempGameResultScreen)
			break;
		else
			continue;
	}

	if (!mVRGameInstanceRef->GetIsFirstLobbyEntryFlag())
	{
		UE_LOG(LogTemp, Error, TEXT("Player is Lobby First Entry!!"));
	}

	if (mVRGameInstanceRef->GetIsGameResultSuccess())
	{
		//UE_LOG(LogTemp, Error, TEXT("Game Result Success Print Logic Execute!"));
		TempGameResultScreen->GameResultDisplay(true);
	}

	if (mVRGameInstanceRef->GetIsGameResultFail())
	{
		//UE_LOG(LogTemp, Error, TEXT("Game Result Fail Print Logic Execute!"));
		TempGameResultScreen->GameResultDisplay(false);
	}
}
