#include "BTBase/BT_Tasks/UAITask_Distract.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_Distract::UUAITask_Distract()
{
	NodeName = TEXT("BTTask_Distract");
	bCreateNodeInstance = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MonFinder_Dance(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/TargetAnim/Montages/Retarget_Dance_Anim_Montage.Retarget_Dance_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MonFinder_Hello(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/TargetAnim/Montages/Retarget_Hello_Anim_Montage.Retarget_Hello_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MonFinder_Pointing(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/TargetAnim/Montages/Retarget_Pointing_Anim_Montage.Retarget_Pointing_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MonFinder_Streching(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/TargetAnim/Montages/Retarget_Stretching_Anim_Montage.Retarget_Stretching_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MonFinder_Sleep(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/TargetAnim/Montages/Retarget_Sleep_Anim_Montage.Retarget_Sleep_Anim_Montage"));

	if (MonFinder_Dance.Succeeded() && MonFinder_Hello.Succeeded() && MonFinder_Pointing.Succeeded() && MonFinder_Streching.Succeeded() && MonFinder_Sleep.Succeeded())
	{
		DistractMontageArrs.Add(MonFinder_Dance.Object);
		DistractMontageArrs.Add(MonFinder_Hello.Object);
		DistractMontageArrs.Add(MonFinder_Pointing.Object);
		DistractMontageArrs.Add(MonFinder_Streching.Object);
		DistractMontageArrs.Add(MonFinder_Sleep.Object);
	}
}

EBTNodeResult::Type UUAITask_Distract::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;
	//UE_LOG(LogTemp, Log, TEXT("UAI_Task Distract Execute!!"));

	int32 RanPlayMontageNum = FMath::FMath::RandRange(0, DistractMontageArrs.Num() - 1);

	//UE_LOG(LogTemp, Log, TEXT("Select Mon Num : %d"), RanPlayMontageNum);

	UAnimMontage* CurrPlayMontage = DistractMontageArrs[RanPlayMontageNum];

	if (MyAnimInst)
	{
		MyAnimInst->Montage_Play(CurrPlayMontage);
		MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_Distract::OnDistractMontageEnded);
		MyAnimInst->OnMontageEnded.AddDynamic(this, &UUAITask_Distract::OnDistractMontageEnded);
	}

	return EBTNodeResult::InProgress;
}

void UUAITask_Distract::OnDistractMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//UE_LOG(LogTemp, Log, TEXT("Distract Montage Play End!!!"));

	PrisonerControllerObj->HandleNextTask();
	MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_Distract::OnDistractMontageEnded);
	return;
}
