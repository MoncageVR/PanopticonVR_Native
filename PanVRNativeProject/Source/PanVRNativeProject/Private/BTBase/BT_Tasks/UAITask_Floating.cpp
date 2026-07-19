// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBase/BT_Tasks/UAITask_Floating.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "Kismet/GameplayStatics.h"

UUAITask_Floating::UUAITask_Floating()
{
	NodeName = TEXT("BTTask_Floating");
	bCreateNodeInstance = true;
	FloatingExecutionTime = 30.0f;
}

EBTNodeResult::Type UUAITask_Floating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;
	//UE_LOG(LogTemp, Log, TEXT("UAI_Task Floating Execute!!"));

	FloatingTargetVec = PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("TopEscapeTargetVec"));
	PrisonerCharacterObj->GetCapsuleComponent()->SetEnableGravity(true);
	PrisonerCharacterObj->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	this->ActuallyMoveToTargetVec();

	return EBTNodeResult::InProgress;

}

void UUAITask_Floating::OnFloatingFinishedFunc()
{
	UE_LOG(LogTemp, Log, TEXT("Floating Arrived!"));
	PrisonerControllerObj->OnTaskFinished.Broadcast();
}

void UUAITask_Floating::ActuallyMoveToTargetVec()
{
	// 2 = UpperState : Move , 6 = LowerState : Floating
	PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2, 6);

	bool TempNearlyFlag = false;
	TempNearlyFlag = FloatingTargetVec.Equals(PrisonerCharacterObj->GetCapsuleComponent()->GetComponentLocation(), 10.0f);

	FloatingStateLatentInfo.CallbackTarget = this;
	FloatingStateLatentInfo.Linkage = 0;
	FloatingStateLatentInfo.ExecutionFunction = FName("OnFloatingFinishedFunc");
	FloatingStateLatentInfo.UUID = 1003;

	if (!TempNearlyFlag)
	{
		UKismetSystemLibrary::MoveComponentTo(
			PrisonerCharacterObj->GetCapsuleComponent(),
			FloatingTargetVec,
			FRotator(0.0f, 180.f, 0.0f),
			false,
			false,
			FloatingExecutionTime,
			false,
			EMoveComponentAction::Move,
			FloatingStateLatentInfo
		);
	}
}
