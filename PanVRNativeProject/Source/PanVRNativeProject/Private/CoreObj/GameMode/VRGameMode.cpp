#include "CoreObj/GameMode/VRGameMode.h"
#include "CoreObj/Manager/VRGameInstance.h"
#include "CoreObj/Manager/GameInstanceSubSystem/PrisonerManagerSubsystem.h"
#include "CoreObj/Manager/GameInstanceSubSystem/MapObjManagerSubsystem.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreCommon/PrisonerRelated/PrisonerCharacter.h"
#include "CoreCommon/PrisonerRelated/PrisonerController.h"
#include "CoreCommon/AmbientSound/AAmbientSound.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CoreCommon/VRPawn/VRCharacterBase.h"
#include "CoreCommon/VRPawn/CVRPawn.h"
#include "Kismet/GameplayStatics.h"

void AVRGameMode::InitGameState() { }

AVRGameMode::AVRGameMode()
{
	DefaultPawnClass = ACVRPawn::StaticClass();
	bIsGameOverFlag = false;
	bIsGameSuccessFlag = false;
	MyVRPawn = nullptr;
}

void AVRGameMode::StartPlay()
{
	Super::StartPlay();

	this->ClearGameResultFlag();

	MyVRGameInstance = Cast<UVRGameInstance>(GetWorld()->GetGameInstance());
	if (MyVRGameInstance)
	{
		UPrisonerMgrSubSyPtr = MyVRGameInstance->GetPrisonerManager();
		UMapObjMgrSubSyPtr = MyVRGameInstance->GetMapObjManager();
		VREquipWorldSubSyPtr = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
		check(UPrisonerMgrSubSyPtr);
		check(UMapObjMgrSubSyPtr);
		check(VREquipWorldSubSyPtr);

		UPrisonerMgrSubSyPtr->HandleClearVarsForGameStart();
		UPrisonerMgrSubSyPtr->CreateAllPrisoner();
		//UPrisonerMgrSubSyPtr->Create_Paranormal_Phenomenon(); // Debug

		UMapObjMgrSubSyPtr->CreateAllGratings();

		VREquipWorldSubSyPtr->FGameStartSignature.AddDynamic(this, &AVRGameMode::HandleGMReceiveByGTW);
	}

	MyVRPawn = Cast<ACVRPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (UWorld* MyWorld = GetWorld())
	{
		FTransform TempSpawnTransform(FTransform::Identity);
		AAAmbientSound* TempAmbientSoundActor = MyWorld->SpawnActor<AAAmbientSound>(AAAmbientSound::StaticClass(), TempSpawnTransform);
		if (TempAmbientSoundActor)
		{
			TempAmbientSoundActor->SetPlayingSound(0);
		}
	}
}

void AVRGameMode::GameOverCheckEvent()
{
	if (bIsGameOverFlag)
	{
		if (MyVRPawn)
		{
			MyVRGameInstance->SetIsGameResultFail(true);
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

void AVRGameMode::GameSuccessCheckEvent()
{
	if (bIsGameSuccessFlag)
	{
		MyVRGameInstance->SetIsGameResultSuccess(true);
		MyVRPawn->HandleDownMovePlayer();
	}
	else
	{
		return;
	}
}

void AVRGameMode::ClearGameResultFlag()
{
	bIsGameOverFlag = false;
	bIsGameSuccessFlag = false;
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
	if (InHandleFlag) // InHandleFlag Value Is Valid(1,2 ..)
	{
		if (InHandleFlag >= 2)
		{ // InHandleFlag Value is 2 More = Flame Status Prisoner Subdue By Firealarm
			ListOfPrisonersWithFlameStatus.Empty();
		}
		else // InHandleFlag Value is 1 = Flame Status Prisoner to be Added
		{
			ListOfPrisonersWithFlameStatus.AddUnique(InFlameUniqueNum);
			if (ListOfPrisonersWithFlameStatus.Num() >= 5)
			{
				// GameOver Logic Execute Parts!
				this->SetIsGameOverFlag(true);
				this->GameOverCheckEvent();

				return;
			}
		}
	}
	else // InHandleFlag Value Is Not Valid(0)
	{
		ListOfPrisonersWithFlameStatus.RemoveSingleSwap(InFlameUniqueNum);
	}
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
