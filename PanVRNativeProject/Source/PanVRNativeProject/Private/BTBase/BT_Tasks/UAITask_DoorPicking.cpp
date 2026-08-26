#include "BTBase/BT_Tasks/UAITask_DoorPicking.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "CoreObj/Manager/GameInstanceSubSystem/PrisonerManagerSubsystem.h"
#include "CoreObj/Manager/GameInstanceSubSystem/MapObjManagerSubsystem.h"
#include "MainActor/AGrating.h"

UUAITask_DoorPicking::UUAITask_DoorPicking()
{
	NodeName = TEXT("BTTask_DoorPicking");
	bCreateNodeInstance = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AMFinder_DoorPicking(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/TargetAnim/Montages/Retarget_DoorPicking_Anim_Montage.Retarget_DoorPicking_Anim_Montage"));
	if (AMFinder_DoorPicking.Succeeded())
	{
		DoorPickingMontage = AMFinder_DoorPicking.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_DoorPicking(TEXT("/Game/VRContent/Sound/Wavs/PrisonerRelated/DoorPicking/sfx_lockpick.sfx_lockpick"));
	if (SFXFinder_DoorPicking.Succeeded())
		SFX_DoorPicking = SFXFinder_DoorPicking.Object;
}

EBTNodeResult::Type UUAITask_DoorPicking::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	if (PrisonerManagerSubSystemInst)
	{
		int32 TempUniqueNum = AdjustPrisonerUniqueNum(PrisonerControllerObj->GetBBComp()->GetValueAsInt(TEXT("UniqueNum")));
		CalMontagePlayTransform(PrisonerManagerSubSystemInst->GetBaseSpawnRotations()[TempUniqueNum].Yaw);
		MontagePlayRot = PrisonerManagerSubSystemInst->GetFinalAllSpawnRoations()[TempUniqueNum];
	}

	if (!ensure(MapObjManagerSubSystemInst)) return EBTNodeResult::Failed;

	if (!MyAnimInst || !DoorPickingMontage)
		return EBTNodeResult::Failed;

	if (MyAnimInst)
	{
		CachedOwnerComp = &OwnerComp;
		PrisonerCharacterObj->GetRootComponent()->SetWorldLocationAndRotation(MontagePlayVec, MontagePlayRot);
		MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_DoorPicking::OnDoorPickingMontageEnded);
		MyAnimInst->OnMontageEnded.AddDynamic(this, &UUAITask_DoorPicking::OnDoorPickingMontageEnded);
		MyAnimInst->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UUAITask_DoorPicking::OnMontageNotify);
		MyAnimInst->OnPlayMontageNotifyBegin.AddDynamic(this, &UUAITask_DoorPicking::OnMontageNotify);
		MyAnimInst->Montage_Play(DoorPickingMontage);
	}

	return EBTNodeResult::InProgress;
}

void UUAITask_DoorPicking::OnDoorPickingMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == DoorPickingMontage)
	{
		if (MapObjManagerSubSystemInst)
		{
			int TempIndex = PrisonerControllerObj->GetBBComp()->GetValueAsInt(FName("UniqueNum"));
			AAGrating* TempGrating = MapObjManagerSubSystemInst->GetGratingsMap()[TempIndex];
			if (TempGrating)
				TempGrating->GratingOpen();
			else
				return;
		}

		if (MyAnimInst)
		{
			MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_DoorPicking::OnDoorPickingMontageEnded);
			MyAnimInst->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UUAITask_DoorPicking::OnMontageNotify);
		}

		if (PrisonerControllerObj)
			PrisonerControllerObj->OnTaskFinished.Broadcast();

		//if (CachedOwnerComp)
		//{
		//	EBTNodeResult::Type Result = bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
		//	//FinishLatentTask(*CachedOwnerComp, Result);
		//}
	}
}

void UUAITask_DoorPicking::OnMontageNotify(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == FName("PlaySoundLockPick"))
	{
		HVRSoundPlayer::PlaySoundEffect(PrisonerCharacterObj, SFX_DoorPicking, PrisonerCharacterObj->GetRootComponent()->GetComponentLocation());
	}
}

void UUAITask_DoorPicking::CalMontagePlayTransform(float InSpawnYaw)
{
	float TempRadianAngle = FMath::DegreesToRadians(InSpawnYaw);

	MontagePlayVec = FVector(
		FMath::Cos(TempRadianAngle) * 1800.0f,
		FMath::Sin(TempRadianAngle) * 1800.0f,
		PrisonerCharacterObj->GetRootComponent()->GetComponentLocation().Z
	);
}

int32 UUAITask_DoorPicking::AdjustPrisonerUniqueNum(int32 InBBUniqueNum)
{
	if (InBBUniqueNum >= 16)
		return (InBBUniqueNum - 16);
	else if (InBBUniqueNum >= 8)
		return (InBBUniqueNum - 8);
	else
		return InBBUniqueNum;
}
