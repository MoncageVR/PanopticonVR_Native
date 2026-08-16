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

	MyVRGameInstance = Cast<UVRGameInstance>(GetWorld()->GetGameInstance());
	if (MyVRGameInstance)
	{
		UPrisonerMgrSubSyPtr = MyVRGameInstance->GetPrisonerManager();
		UMapObjMgrSubSyPtr = MyVRGameInstance->GetMapObjManager();
		VREquipWorldSubSyPtr = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
		check(UPrisonerMgrSubSyPtr);
		check(UMapObjMgrSubSyPtr);
		check(VREquipWorldSubSyPtr);

		UPrisonerMgrSubSyPtr->CreateAllPrisoner();
		//UPrisonerMgrSubSyPtr->Create_Paranormal_Phenomenon();

		//UMapObjMgrSubSyPtr->CreateAllGratings();

		VREquipWorldSubSyPtr->FGameStartSignature.AddDynamic(this, &AVRGameMode::HandleGMReceiveByGTW);
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

void AVRGameMode::HandleListOfFloatNTelePrisoners(uint8 InHandleFlag, int32 InUniqueNum)
{
	if (InHandleFlag)
		ListOfPrisonersWithFloatingORTeleportStatus.AddUnique(InUniqueNum);
	else
		ListOfPrisonersWithFloatingORTeleportStatus.RemoveSingleSwap(InUniqueNum);
}

void AVRGameMode::HandleListOfFlamePrisoners(uint8 InHandleFlag, int32 InFlameUniqueNum)
{
	UE_LOG(LogTemp, Log, TEXT("InHandleFlag : %d"), InHandleFlag);
	if (InHandleFlag)
	{
		if (InHandleFlag >= 2)
			ListOfPrisonersWithFlameStatus.Empty();
		else
			ListOfPrisonersWithFlameStatus.AddUnique(InFlameUniqueNum);
	}
	else
		ListOfPrisonersWithFlameStatus.RemoveSingleSwap(InFlameUniqueNum);
}

void AVRGameMode::HandleGMReceiveByGTW(bool bIsGameStartFlag)
{
	if (bIsGameStartFlag)
	{
		UE_LOG(LogTemp, Log, TEXT("VRGameMode Start  By GTWLever"));

		for (APrisonerController* Controller : UPrisonerMgrSubSyPtr->GetAllPrisonerControllerArr())
		{
			Controller->HandleRunBT();
		}

	}
}
