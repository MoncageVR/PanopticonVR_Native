#include "BTBase/BT_Tasks/UAITask_Default.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_Default::UUAITask_Default()
{
	NodeName = TEXT("BTTask_Default");
	bCreateNodeInstance = true;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_DefaultWalk(TEXT("/Game/VRContent/Sound/Wavs/PrisonerRelated/Run/sfx_Defaultwalk.sfx_Defaultwalk"));
	if (SFXFinder_DefaultWalk.Succeeded())
		SFX_DefaultWalk = SFXFinder_DefaultWalk.Object;
}

EBTNodeResult::Type UUAITask_Default::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;

	SpawnPosition = PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("SpawnVec"));

	FNavLocation PrisonerSpawnPosition;
	UNavigationSystemV1::GetNavigationSystem(GetWorld())->GetRandomReachablePointInRadius(SpawnPosition, 250.f, PrisonerSpawnPosition);

	if (PrisonerSpawnPosition.HasNodeRef())
	{
		PrisonerCharacterObj->HandlePlayAPSound(SFX_DefaultWalk);
		PatrolTargetPosition = PrisonerSpawnPosition.Location;
		PrisonerControllerObj->GetBBComp()->SetValueAsVector(TEXT("PatrolTargetVec"), PatrolTargetPosition);

		float TempSpeed = (PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed")) - 1.0f);

		PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = TempSpeed;
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
