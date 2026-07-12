#include "BTBase/BT_Tasks/UAITask_Anger.h"
#include "CoreObj/Manager/PrisonerManagerSubsystem.h"
#include "CoreObj/Manager/MapObjManagerSubsystem.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_Anger::UUAITask_Anger()
{
	NodeName = TEXT("BTTask_Anger");
	bCreateNodeInstance = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AMFinder_Anger(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/TargetAnim/Montages/Retarget_Anger_Anim_Montage.Retarget_Anger_Anim_Montage"));
	if (AMFinder_Anger.Succeeded())
		AngerMontage = AMFinder_Anger.Object;
	else
		AngerMontage = nullptr;
}

EBTNodeResult::Type UUAITask_Anger::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;

	this->CalMonPlayTransform();

	MapObjManagerSubSystemInst = GetWorld()->GetGameInstance()->GetSubsystem<UMapObjManagerSubsystem>();
	PrisonerCharacterObj->GetCapsuleComponent()->SetWorldLocationAndRotation(MonPlayVec, MonPlayRot); // Execute For Anger State, Move the Prisoner to Spawn Location

	CachedOwnerComp = &OwnerComp;

	MyAnimInst = PrisonerCharacterObj->GetMesh()->GetAnimInstance();
	if (!ensure(MyAnimInst)) return EBTNodeResult::Failed;
	MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_Anger::OnAngerMontageEnded);
	MyAnimInst->OnMontageEnded.AddDynamic(this, &UUAITask_Anger::OnAngerMontageEnded);
	MyAnimInst->Montage_Play(AngerMontage);

	GetWorld()->GetTimerManager().SetTimer(
		DelayGratingFlyTimer,
		this,
		&UUAITask_Anger::CallHandleGratingFly,
		1.5f,
		false
	);

	return EBTNodeResult::InProgress;
}

void UUAITask_Anger::OnAngerMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AngerMontage)
	{
		GetWorld()->GetTimerManager().ClearTimer(DelayGratingFlyTimer);

		if (MyAnimInst)
			MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_Anger::OnAngerMontageEnded);

		if (PrisonerControllerObj)
		{
			EBTNodeResult::Type Result = bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
			PrisonerControllerObj->OnTaskFinished.Broadcast();
			FinishLatentTask(*CachedOwnerComp, Result);
		}
	}
}

void UUAITask_Anger::CalMonPlayTransform()
{
	UPrisonerManagerSubsystem* TempPrisonerManagerInst = GetWorld()->GetGameInstance()->GetSubsystem<UPrisonerManagerSubsystem>();
	if (!ensure(TempPrisonerManagerInst)) return;

	int32 TempUniqueNum = PrisonerControllerObj->GetBBComp()->GetValueAsInt(TEXT("UniqueNum"));

	MonPlayVec = FVector(
		TempPrisonerManagerInst->GetFinalAllSpawnPositions()[TempUniqueNum].X,
		TempPrisonerManagerInst->GetFinalAllSpawnPositions()[TempUniqueNum].Y,
		PrisonerCharacterObj->GetCapsuleComponent()->GetComponentLocation().Z
	);
	MonPlayRot = FRotator(TempPrisonerManagerInst->GetFinalAllSpawnRoations()[TempUniqueNum]);
}

void UUAITask_Anger::CallHandleGratingFly()
{
	MapObjManagerSubSystemInst->HandleFlyingTheGrating(PrisonerControllerObj->GetBBComp()->GetValueAsInt(TEXT("UniqueNum")));
}
