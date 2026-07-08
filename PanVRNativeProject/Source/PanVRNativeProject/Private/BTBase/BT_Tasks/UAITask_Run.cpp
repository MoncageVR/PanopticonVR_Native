#include "BTBase/BT_Tasks/UAITask_Run.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_Run::UUAITask_Run()
{
	NodeName = TEXT("BTTask_Run");
}

EBTNodeResult::Type UUAITask_Run::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}
	
	if (HasReachedTargetPos(
		PrisonerCharacterObj->GetRootComponent()->GetComponentLocation(),
		PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("EscapeTargetVec"))))
	{
		//UE_LOG(LogTemp, Log, TEXT("UAI_Task_Run Success Execute!!"));
		PrisonerControllerObj->OnTaskFinished.Broadcast();
		//return EBTNodeResult::Succeeded;
	}
	else
	{
		PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));
		return EBTNodeResult::Succeeded;
	}
	

	//UE_LOG(LogTemp, Log, TEXT("UAI_Task_Run Execute!!"));
	//PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));
	return EBTNodeResult::Succeeded;
}

const bool UUAITask_Run::HasReachedTargetPos(const FVector InChaVec, const FVector InTargetVec)
{
	bool XReturnValue, YReturnValue, ZReturnValue = false;
	if (FMath::IsNearlyEqual(InChaVec.X, InTargetVec.X, 1.0f))
		XReturnValue = true;

	if (FMath::IsNearlyEqual(InChaVec.Y, InTargetVec.Y, 1.0f))
		YReturnValue = true;

	if (FMath::IsNearlyEqual(InChaVec.Z, InTargetVec.Z, 10.0f))
		ZReturnValue = true;

	return (XReturnValue && YReturnValue && ZReturnValue);
}