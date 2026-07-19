#include "BTBase/BT_Tasks/UAITask_Escape.h"
#include "CoreObj/Manager/MapObjManagerSubsystem.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_Escape::UUAITask_Escape()
{
	NodeName = TEXT("BTTask_Escape");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UUAITask_Escape::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	// Animation : 4 : Dangerous(UpperState)
	PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(4);

	FTimerHandle TempTimer;
	GetWorld()->GetTimerManager().SetTimer(
		TempTimer,
		this,
		&UUAITask_Escape::CallControlExitDoorFunction,
		17.9f,
		false
	);

	return EBTNodeResult::Succeeded;
}

void UUAITask_Escape::CallControlExitDoorFunction()
{
	MapObjManagerSubSystemInst->ControlExitDoorFunction();
}
