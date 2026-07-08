// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBase/BT_Tasks/UAITask_Escape.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_Escape::UUAITask_Escape()
{
	NodeName = TEXT("BTTask_Escape");
}

EBTNodeResult::Type UUAITask_Escape::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	UE_LOG(LogTemp, Log, TEXT("UAI_Task_Escape Execute!!"));

	return EBTNodeResult::InProgress;
}
