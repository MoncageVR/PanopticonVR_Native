// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreObj/GameMode/VRGameMode.h"
#include "CoreObj/VRGameInstance.h"
#include "CoreObj/Manager/PrisonerManagerSubsystem.h"
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
		UPrisonerManagerSubsystem* TempPrisonerManagerPointer = MyVRGameInstance->GetPrisonerManager();

		if (ensure(TempPrisonerManagerPointer))
		{
			TempPrisonerManagerPointer->CreateAllPrisoner();
		}
	}
}