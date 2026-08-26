#include "BTBase/BT_Tasks/UAITask_RandomMove.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_RandomMove::UUAITask_RandomMove()
{
	NodeName = TEXT("BTTask_RandomMove");
	bCreateNodeInstance = true;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_RanMoveWalk(TEXT("/Game/VRContent/Sound/Wavs/PrisonerRelated/Run/sfx_walk.sfx_walk"));
	if (SFXFinder_RanMoveWalk.Succeeded())
		SFX_RanMoveWalk = SFXFinder_RanMoveWalk.Object;
}

EBTNodeResult::Type UUAITask_RandomMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;

	if (HasReachedRandomTargetPos(PrisonerCharacterObj->GetRootComponent()->GetComponentLocation(), PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("RandomMoveTargetVec"))))
	{
		PrisonerCharacterObj->HandlePauseAPSound();
		UE_LOG(LogTemp, Log, TEXT("Random Move Target Vector Reach Success!"));
		// UpperState : Idle(0) | LowerState : Default(0)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(0, 0);
		PrisonerControllerObj->HandleNextTask();
	}
	else
	{
		PrisonerCharacterObj->HandlePlayAPSound(SFX_RanMoveWalk);
		// UpperState : Move(2) | LowerState : Run(4)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2, 4);

		PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));
		PrisonerControllerObj->GetBBComp()->SetValueAsVector(TEXT("RandomMoveTargetVec"), MakeRandomVec());
		return EBTNodeResult::Succeeded;
	}

	// 2 = UpperState : Move , 5 = LowerState : Run
	/*PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2, 5);

	PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));
	PrisonerControllerObj->GetBBComp()->SetValueAsVector(TEXT("RandomMoveTargetVec"), MakeRandomVec());*/

	return EBTNodeResult::InProgress;
}

const FVector UUAITask_RandomMove::MakeRandomVec()
{
	float Radius = FMath::FRandRange(300.f, 1000.f);
	FVector Dir = FMath::VRand();
	FVector Result = Dir * Radius;
	Result.Z = 287.f;
	FVector Center = FVector(0, 0, 0);

	return (Center + Result);
}

const bool UUAITask_RandomMove::HasReachedRandomTargetPos(const FVector InChaVec, const FVector InTargetVec)
{
	bool XReturnValue = false;
	bool YReturnValue = false;
	if (FMath::IsNearlyEqual(InChaVec.X, InTargetVec.X, 5.0f))
		XReturnValue = true;

	if (FMath::IsNearlyEqual(InChaVec.Y, InTargetVec.Y, 5.0f))
		YReturnValue = true;
	return (XReturnValue && YReturnValue);
}
