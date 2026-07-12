#include "BTBase/BT_Tasks/UAITask_Run.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_Run::UUAITask_Run()
{
	NodeName = TEXT("BTTask_Run");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UUAITask_Run::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	// Animation : 2 : Move(UpperState)
	PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2);

	if (HasReachedTargetPos(
		PrisonerCharacterObj->GetRootComponent()->GetComponentLocation(),
		PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("EscapeTargetVec"))))
	{
		PrisonerCharacterObj->GetRootComponent()->SetWorldRotation(FRotator(0.f, 90.0f, 0.f));
		PrisonerControllerObj->OnTaskFinished.Broadcast();
		//return EBTNodeResult::Succeeded;
	}
	else
	{
		PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}

const bool UUAITask_Run::HasReachedTargetPos(const FVector InChaVec, const FVector InTargetVec)
{
	bool XReturnValue, YReturnValue = false;
	if (FMath::IsNearlyEqual(InChaVec.X, InTargetVec.X, 5.0f))
		XReturnValue = true;

	if (FMath::IsNearlyEqual(InChaVec.Y, InTargetVec.Y, 5.0f))
		YReturnValue = true;

	return (XReturnValue && YReturnValue);
}