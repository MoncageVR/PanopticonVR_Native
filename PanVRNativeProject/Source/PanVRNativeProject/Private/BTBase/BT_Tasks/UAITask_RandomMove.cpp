// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBase/BT_Tasks/UAITask_RandomMove.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_RandomMove::UUAITask_RandomMove()
{
	NodeName = TEXT("BTTask_RandomMove");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UUAITask_RandomMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;

	UE_LOG(LogTemp, Log, TEXT("UAI_Task_RandomMove Success Execute!!"));

	// Animation : 2 : Move(UpperState)
	PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2);

	PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));
	PrisonerControllerObj->GetBBComp()->SetValueAsVector(TEXT("RandomMoveTargetVec"), MakeRandomVec());

	return EBTNodeResult::Succeeded;
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
