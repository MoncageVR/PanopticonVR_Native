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

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_Sleep(TEXT("/Game/VRContent/Sound/Wavs/PrisonerRelated/Distract/sfx_sleep.sfx_sleep"));
	if (SFXFinder_Sleep.Succeeded())
		SFX_Sleep = SFXFinder_Sleep.Object;
}

EBTNodeResult::Type UUAITask_Distract::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;

	int32 RanPlayMontageNum = FMath::FMath::RandRange(0, DistractMontageArrs.Num() - 1);

	if (RanPlayMontageNum == 4)
		PrisonerCharacterObj->HandlePlayAPSound(SFX_Sleep);

	CachedOwnerComp = &OwnerComp;

	UAnimMontage* CurrPlayMontage = DistractMontageArrs[RanPlayMontageNum];
	UE_LOG(LogTemp, Log, TEXT("1:Dance,2:Hello,3:Point,4:Strech,5:Sleep - CurrIndex : %d"), RanPlayMontageNum);

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
	MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_Distract::OnDistractMontageEnded);
	PrisonerControllerObj->HandleNextTask();
	if (CachedOwnerComp)
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}

	return;
}
