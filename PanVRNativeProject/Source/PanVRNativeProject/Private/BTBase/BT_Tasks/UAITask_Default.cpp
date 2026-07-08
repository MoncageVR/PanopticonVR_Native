// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBase/BT_Tasks/UAITask_Default.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_Default::UUAITask_Default()
{
	NodeName = TEXT("BTTask_Default");
}

EBTNodeResult::Type UUAITask_Default::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;

	SpawnPosition = PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("SpawnVec"));

	FNavLocation PrisonerSpawnPosition;
	UNavigationSystemV1::GetNavigationSystem(GetWorld())->GetRandomReachablePointInRadius(SpawnPosition, 250.f, PrisonerSpawnPosition);

	if (PrisonerSpawnPosition.HasNodeRef())
	{
		PatrolTargetPosition = PrisonerSpawnPosition.Location;
		PrisonerControllerObj->GetBBComp()->SetValueAsVector(TEXT("PatrolTargetVec"), PatrolTargetPosition);
		PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}

void UUAITask_Default::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
