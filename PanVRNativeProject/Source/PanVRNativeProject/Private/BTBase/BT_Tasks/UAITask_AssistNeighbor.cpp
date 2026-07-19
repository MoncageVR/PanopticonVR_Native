#include "BTBase/BT_Tasks/UAITask_AssistNeighbor.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "CoreObj/Manager/PrisonerManagerSubsystem.h"

UUAITask_AssistNeighbor::UUAITask_AssistNeighbor()
{
	NodeName = TEXT("BTTask_AssistNeighbor");
	bCreateNodeInstance = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AMFinder_NeighborDoorPicking(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/TargetAnim/Montages/Retarget_DoorPicking_Anim_Montage.Retarget_DoorPicking_Anim_Montage"));
	if (AMFinder_NeighborDoorPicking.Succeeded())
	{
		NeighborDoorPickingMontage = AMFinder_NeighborDoorPicking.Object;
	}
}

EBTNodeResult::Type UUAITask_AssistNeighbor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;

	//UE_LOG(LogTemp, Log, TEXT("UAI_Task_AssistNeighbor Execute!!"));

	if (HasReachedNeighborTargetVec(PrisonerCharacterObj->GetRootComponent()->GetComponentLocation(),
		PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("AssistNeighborMoveTargetVec"))))
	{
		UE_LOG(LogTemp, Log, TEXT("Already Neighbor Position Reached"));
		// Animation : 0 : Idle(UpperState)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(0);
		PrisonerCharacterObj->GetRootComponent()->SetWorldRotation(TargetStructureInfo.TargetRotation);

		// Neighbor Door Picking Montage Play Logic Parts

	}
	else
	{
		// Animation : 2 : Move(UpperState)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2);
		PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));
		AdjustTargetPrisonerUniqueNum(PrisonerControllerObj->GetBBComp()->GetValueAsInt(TEXT("UniqueNum")));
		PrisonerControllerObj->GetBBComp()->SetValueAsVector(TEXT("AssistNeighborMoveTargetVec"), TargetStructureInfo.TargetPosition);
	}

	/*UE_LOG(LogTemp, Log, TEXT("Location X:%f , Y:%f , Z:%f | %d | Rotation Roll:%f , Pitch:%f , Yaw:%f"), TargetStructureInfo.TargetPosition.X, TargetStructureInfo.TargetPosition.Y, TargetStructureInfo.TargetPosition.Z, TargetStructureInfo.TargetUniqueNum, TargetStructureInfo.TargetRotation.Roll, TargetStructureInfo.TargetRotation.Pitch, TargetStructureInfo.TargetRotation.Yaw);*/

	return EBTNodeResult::Succeeded;
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
