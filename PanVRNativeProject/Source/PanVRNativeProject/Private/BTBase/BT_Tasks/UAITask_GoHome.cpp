#include "BTBase/BT_Tasks/UAITask_GoHome.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_GoHome::UUAITask_GoHome()
{
	NodeName = TEXT("BTTask_GoHome");
	bCreateNodeInstance = true;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_ReturnWalk(TEXT("/Game/VRContent/Sound/Wavs/PrisonerRelated/Run/sfx_walk.sfx_walk"));
	if (SFXFinder_ReturnWalk.Succeeded())
		SFX_GoHomeWalk = SFXFinder_ReturnWalk.Object;
}

EBTNodeResult::Type UUAITask_GoHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;
	//UE_LOG(LogTemp, Log, TEXT("UAI_Task GoHome Execute!!"));

	PrisonerCharacterObj->HandlePlayAPSound(SFX_GoHomeWalk);

	if (HasReachedGoHomeTargetVec(
		PrisonerCharacterObj->GetRootComponent()->GetComponentLocation(), 
		PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("SpawnVec"))
	))
	{
		// UpperState : Idle(0) | LowerState : Default(0)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(0, 0);
		PrisonerControllerObj->OnTaskFinished.Broadcast();

	}
	else
	{
		// UpperState : Move(2) | LowerState : Run(4)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2, 2);
		float GoHomeRunSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed")) + 60.0f;
		//UE_LOG(LogTemp, Log, TEXT("Current Speed : %f"), GoHomeRunSpeed);

		PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = GoHomeRunSpeed;
	}

	return EBTNodeResult::Succeeded;

}

bool UUAITask_GoHome::HasReachedGoHomeTargetVec(const FVector InChaVec, const FVector InGoHomeTargetVec)
{
	bool XReturnValue = false;
	bool YReturnValue = false;
	if (FMath::IsNearlyEqual(InChaVec.X, InGoHomeTargetVec.X, 5.0f))
		XReturnValue = true;

	if (FMath::IsNearlyEqual(InChaVec.Y, InGoHomeTargetVec.Y, 5.0f))
		YReturnValue = true;

	return (XReturnValue && YReturnValue);
}