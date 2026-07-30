#include "BTBase/BT_Tasks/UAITask_Roar.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_Roar::UUAITask_Roar()
{
	NodeName = TEXT("BTTask_Roar");
	bCreateNodeInstance = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MonFinder_Roar(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/TargetAnim/Montages/Retarget_Roar_Anim_Montage.Retarget_Roar_Anim_Montage"));
	if (MonFinder_Roar.Succeeded())
	{
		RoarMontage = MonFinder_Roar.Object;
	}
}

EBTNodeResult::Type UUAITask_Roar::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;
	UE_LOG(LogTemp, Log, TEXT("UAI_Task Roar Execute!!"));

	if (MyAnimInst)
	{
		MyAnimInst->Montage_Play(RoarMontage);
		MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_Roar::OnRoarMontageEnded);
		MyAnimInst->OnMontageEnded.AddDynamic(this, &UUAITask_Roar::OnRoarMontageEnded);
	}

	return EBTNodeResult::InProgress;
}

void UUAITask_Roar::OnRoarMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Log, TEXT("Roar Montage Play End"));
	PrisonerControllerObj->HandleNextTask();
	MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_Roar::OnRoarMontageEnded);
	return;
}
