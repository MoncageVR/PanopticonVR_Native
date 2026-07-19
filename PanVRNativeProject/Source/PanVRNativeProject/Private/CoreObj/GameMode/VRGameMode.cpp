// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreObj/GameMode/VRGameMode.h"
#include "CoreObj/VRGameInstance.h"
#include "CoreObj/Manager/PrisonerManagerSubsystem.h"
#include "CoreObj/Manager/MapObjManagerSubsystem.h"
#include "Core/Character/PrisonerCharacter.h"
#include "Core/Character/PrisonerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "VRPawn/VRCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "VRPawn/CVRPawn.h"

void AVRGameMode::InitGameState()
{
	//DefaultPawnClass = 
}

AVRGameMode::AVRGameMode()
{
	DefaultPawnClass = ACVRPawn::StaticClass();
	bIsGameOverFlag = false;
	MyVRPawn = nullptr;

	//static ConstructorHelpers::FClassFinder<AActor> PanwFinder_Spectator(TEXT("/Game/VRSpectator/VRSpectator.VRSpectator_C"));
	//if (PanwFinder_Spectator.Succeeded())
	//{
	//	//SpectatorClass = PanwFinder_Spectator.Class;
	//}
}

void AVRGameMode::StartPlay()
{
	Super::StartPlay();

	MyVRGameInstance = Cast<UVRGameInstance>(GetWorld()->GetGameInstance());
	if (MyVRGameInstance)
	{
		UPrisonerManagerSubsystem* TempPrisonerManagerPtr = MyVRGameInstance->GetPrisonerManager();

		UMapObjManagerSubsystem* TempMapObjManangerPtr = MyVRGameInstance->GetMapObjManager();

		if (ensure(TempPrisonerManagerPtr))
		{
			TempPrisonerManagerPtr->CreateAllPrisoner();
			TempPrisonerManagerPtr->Create_Paranormal_Phenomenon();
		}

		if (ensure(TempMapObjManangerPtr))
		{
			TempMapObjManangerPtr->CreateAllGratings();
		}
	}

	MyVRPawn = Cast<ACVRPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void AVRGameMode::GameOverCheckEvent()
{
	if (bIsGameOverFlag)
	{
		if (MyVRPawn)
		{
			MyVRPawn->HandleDownMovePlayer();
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}
}