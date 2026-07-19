// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBase/BT_Tasks/UAITask_Teleport.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_Teleport::UUAITask_Teleport()
{
	NodeName = TEXT("BTTask_Teleport");
	bCreateNodeInstance = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AMFinder_Teleport(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/TargetAnim/Montages/Retarget_Teleport_Anim_Montage.Retarget_Teleport_Anim_Montage"));
	if (AMFinder_Teleport.Succeeded())
		TeleportingMontage = AMFinder_Teleport.Object;
}

EBTNodeResult::Type UUAITask_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;
	//UE_LOG(LogTemp, Log, TEXT("UAI_Task_Teleport Execute!!"));

	PrisonerCharacterObj->GetRootComponent()->SetWorldLocationAndRotation(PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("TeleportTargetVec")), FRotator(0.0f, 180.0f, 0.0f));

	MyAnimInst->Montage_Play(TeleportingMontage);
	if (MyAnimInst)
	{
		MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_Teleport::OnTeleportMontageEnded);
		MyAnimInst->OnMontageEnded.AddDynamic(this, &UUAITask_Teleport::OnTeleportMontageEnded);
	}
	return EBTNodeResult::InProgress;
}

void UUAITask_Teleport::OnTeleportMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//UE_LOG(LogTemp, Log, TEXT("Teleport Montage Play Ended"));

	PrisonerControllerObj->HandleNextTask();
	MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_Teleport::OnTeleportMontageEnded);

	return;
}
