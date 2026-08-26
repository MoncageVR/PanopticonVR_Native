#include "BTBase/BT_Tasks/UAITask_AssistNeighbor.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "CoreObj/Manager/GameInstanceSubSystem/PrisonerManagerSubsystem.h"
#include "CoreObj/Manager/GameInstanceSubSystem/MapObjManagerSubsystem.h"
#include "MainActor/AGrating.h"

UUAITask_AssistNeighbor::UUAITask_AssistNeighbor()
{
	NodeName = TEXT("BTTask_AssistNeighbor");
	bCreateNodeInstance = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AMFinder_NeighborDoorPicking(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/TargetAnim/Montages/Retarget_DoorPicking_Anim_Montage.Retarget_DoorPicking_Anim_Montage"));
	if (AMFinder_NeighborDoorPicking.Succeeded())
	{
		NeighborDoorPickingMontage = AMFinder_NeighborDoorPicking.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_DoorPickingForNeighbor(TEXT("/Game/VRContent/Sound/Wavs/PrisonerRelated/DoorPicking/sfx_lockpick.sfx_lockpick"));
	if (SFXFinder_DoorPickingForNeighbor.Succeeded())
		SFX_DoorPickingForNeighbor = SFXFinder_DoorPickingForNeighbor.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_WalkForNeighbor(TEXT("/Game/VRContent/Sound/Wavs/PrisonerRelated/Run/sfx_walk.sfx_walk"));
	if (SFXFinder_WalkForNeighbor.Succeeded())
		SFX_WalkForNeighbor = SFXFinder_WalkForNeighbor.Object;

	GrantedUpperStates.Add(2);
	GrantedUpperStates.Add(4);
	GrantedLowerStates.Add(4);
	GrantedLowerStates.Add(12);
}

EBTNodeResult::Type UUAITask_AssistNeighbor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;

	if (HasReachedNeighborTargetVec(PrisonerCharacterObj->GetRootComponent()->GetComponentLocation(),
		PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("AssistNeighborMoveTargetVec"))))
	{
		PrisonerCharacterObj->HandlePauseAPSound();
		// UpperState : Idle(0) | LowerState : Default(0)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(0, 0);
		PrisonerCharacterObj->GetRootComponent()->SetWorldRotation(TargetStructureInfo.TargetRotation);

		// Neighbor Door Picking Montage Play Logic Parts
		if (!ensure(MyAnimInst)) return EBTNodeResult::Failed;
		if (MyAnimInst)
		{
			MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_AssistNeighbor::OnAssistNeighborMontageEnded);
			MyAnimInst->OnMontageEnded.AddDynamic(this, &UUAITask_AssistNeighbor::OnAssistNeighborMontageEnded);

			MyAnimInst->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UUAITask_AssistNeighbor::OnDoorPickingMontageNotify);
			MyAnimInst->OnPlayMontageNotifyBegin.AddDynamic(this, &UUAITask_AssistNeighbor::OnDoorPickingMontageNotify);

			if (!MyAnimInst->Montage_IsPlaying(NeighborDoorPickingMontage))
				MyAnimInst->Montage_Play(NeighborDoorPickingMontage);
		}
	}
	else
	{
		PrisonerCharacterObj->HandlePlayAPSound(SFX_WalkForNeighbor);
		// UpperState : Move(2) | LowerState : Run(4)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2, 4);
		PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));
		AdjustTargetPrisonerUniqueNum(PrisonerControllerObj->GetBBComp()->GetValueAsInt(TEXT("UniqueNum")));
		PrisonerControllerObj->GetBBComp()->SetValueAsVector(TEXT("AssistNeighborMoveTargetVec"), TargetStructureInfo.TargetPosition);
	}

	return EBTNodeResult::Succeeded;
}

void UUAITask_AssistNeighbor::OnAssistNeighborMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == NeighborDoorPickingMontage)
	{
		if (!ensure(MapObjManagerSubSystemInst)) return;
		AAGrating* TempGrating = MapObjManagerSubSystemInst->GetGratingsMap()[TargetStructureInfo.TargetUniqueNum];
		TempGrating->GratingOpen();

		APrisonerController* Temp = PrisonerManagerSubSystemInst->GetAllPrisonerControllerArr()[TargetStructureInfo.TargetUniqueNum];

		Temp->State_based_ExecutionTasks_GiventoSomeone(GrantedUpperStates, GrantedLowerStates);

		if (MyAnimInst)
		{
			MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_AssistNeighbor::OnAssistNeighborMontageEnded);
			MyAnimInst->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UUAITask_AssistNeighbor::OnDoorPickingMontageNotify);
			MyAnimInst->Montage_Pause(NeighborDoorPickingMontage);
			MyAnimInst->Montage_Stop(0.25f, NeighborDoorPickingMontage);
		}

		if (PrisonerControllerObj)
			PrisonerControllerObj->OnTaskFinished.Broadcast();
	}
}

void UUAITask_AssistNeighbor::OnDoorPickingMontageNotify(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == FName("PlaySoundLockPick"))
	{
		HVRSoundPlayer::PlaySoundEffect(PrisonerCharacterObj, SFX_DoorPickingForNeighbor, PrisonerCharacterObj->GetRootComponent()->GetComponentLocation());
	}
}

void UUAITask_AssistNeighbor::AdjustTargetPrisonerUniqueNum(int32 InMyUniqueNum)
{
	int32 Base = (InMyUniqueNum / 8) * 8;
	int32 Offset = InMyUniqueNum % 8;
	Offset = (Offset - 1 + 8) % 8;
	int32 TempTargetUniqueNum = Base + Offset;
	TargetStructureInfo = AdjustTargetMovePos(TempTargetUniqueNum);
	return;
}

FNeighborInfo UUAITask_AssistNeighbor::AdjustTargetMovePos(int32 InTargetPrisonerNum)
{
	int32 TempNum = 0;
	if (InTargetPrisonerNum >= 16)
		TempNum = InTargetPrisonerNum - 16;
	else if (InTargetPrisonerNum >= 8)
		TempNum = InTargetPrisonerNum - 8;
	else
		TempNum = InTargetPrisonerNum;

	UPrisonerManagerSubsystem* TempPrisonerMananger = GetWorld()->GetGameInstance()->GetSubsystem<UPrisonerManagerSubsystem>();

	float TempRadianAngle = FMath::DegreesToRadians(TempPrisonerMananger->GetBaseSpawnRotations()[TempNum].Yaw);
	FNeighborInfo TempTargetStructureInfo = FNeighborInfo(
		FVector(
			FMath::Cos(TempRadianAngle) * 1425.0f,
			FMath::Sin(TempRadianAngle) * 1425.0f,
			PrisonerCharacterObj->GetRootComponent()->GetComponentLocation().Z
		),
		InTargetPrisonerNum,
		TempPrisonerMananger->GetBaseSpawnRotations()[TempNum]
	);

	return TempTargetStructureInfo;
}

const bool UUAITask_AssistNeighbor::HasReachedNeighborTargetVec(const FVector InChaVec, const FVector InTargetVec)
{
	bool XReturnValue, YReturnValue = false;
	if (FMath::IsNearlyEqual(InChaVec.X, InTargetVec.X, 5.0f))
		XReturnValue = true;
	if (FMath::IsNearlyEqual(InChaVec.Y, InTargetVec.Y, 5.0f))
		YReturnValue = true;
	return (XReturnValue && YReturnValue);
}

FNeighborInfo::FNeighborInfo()
{
	TargetPosition = FVector::Zero();
	TargetUniqueNum = 0;
	TargetRotation = FRotator::ZeroRotator;
}
