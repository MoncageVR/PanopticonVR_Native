#include "CoreObj/GameMode/VRGameMode.h"
#include "CoreObj/Manager/VRGameInstance.h"
#include "CoreObj/Manager/GameInstanceSubSystem/PrisonerManagerSubsystem.h"
#include "CoreObj/Manager/GameInstanceSubSystem/MapObjManagerSubsystem.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreCommon/PrisonerRelated/PrisonerCharacter.h"
#include "CoreCommon/PrisonerRelated/PrisonerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CoreCommon/VRPawn/VRCharacterBase.h"
#include "CoreCommon/VRPawn/CVRPawn.h"
#include "Kismet/GameplayStatics.h"

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

	/*UVREquipmentWorldSubsystem* TempVREquipmentWorldSubSystem = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	check(TempVREquipmentWorldSubSystem);*/
	//TempVREquipmentWorldSubSystem->ClearEquipmentArrs();

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