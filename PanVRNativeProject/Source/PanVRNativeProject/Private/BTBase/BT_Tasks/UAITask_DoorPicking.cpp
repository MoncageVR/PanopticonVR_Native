// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBase/BT_Tasks/UAITask_DoorPicking.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_DoorPicking::UUAITask_DoorPicking()
{
	NodeName = TEXT("BTTask_DoorPicking");

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AMFinder_DoorPicking(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/TargetAnim/Montages/Retarget_DoorPicking_Anim_Montage.Retarget_DoorPicking_Anim_Montage"));
	if (AMFinder_DoorPicking.Succeeded())
	{
		DoorPickingMontage = AMFinder_DoorPicking.Object;
	}
}

EBTNodeResult::Type UUAITask_DoorPicking::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	UAnimInstance* AnimInst = PrisonerCharacterObj->GetMesh()->GetAnimInstance();
	UE_LOG(LogTemp, Log, TEXT("UAI_Task_DoorPicking Execute!!"));
	AnimInst->Montage_Play(DoorPickingMontage);
	
	// You need to create logic to execute the Broadcast function after the montage playback ends.
	// PrisonerControllerObj->OnTaskFinished.Broadcast();

	return EBTNodeResult::InProgress;
}
