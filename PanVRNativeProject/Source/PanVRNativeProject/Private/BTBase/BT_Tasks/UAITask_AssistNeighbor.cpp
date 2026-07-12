#include "BTBase/BT_Tasks/UAITask_AssistNeighbor.h"

UUAITask_AssistNeighbor::UUAITask_AssistNeighbor()
{
	NodeName = TEXT("BTTask_AssistNeighbor");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UUAITask_AssistNeighbor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;

	UE_LOG(LogTemp, Log, TEXT("UAI_Task_AssistNeighbor Execute!!"));

	return EBTNodeResult::Succeeded;
}
