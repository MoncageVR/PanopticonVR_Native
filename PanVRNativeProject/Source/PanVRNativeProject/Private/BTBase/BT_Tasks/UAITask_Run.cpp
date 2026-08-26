#include "BTBase/BT_Tasks/UAITask_Run.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "NavigationPath.h"

UUAITask_Run::UUAITask_Run()
{
	NodeName = TEXT("BTTask_Run");
	bCreateNodeInstance = true;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_Walk(TEXT("/Game/VRContent/Sound/Wavs/PrisonerRelated/Run/sfx_walk.sfx_walk"));
	if (SFXFinder_Walk.Succeeded())
		SFX_Walking = SFXFinder_Walk.Object;
}

EBTNodeResult::Type UUAITask_Run::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}
	PrisonerCharacterObj->HandlePlayAPSound(SFX_Walking);

	if (HasReachedTargetPos(
		PrisonerCharacterObj->GetRootComponent()->GetComponentLocation(),
		PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("EscapeTargetVec"))))
	{
		//UE_LOG(LogTemp, Error, TEXT("Run Move To Escape Taget Pos Reached!"))
		PrisonerCharacterObj->GetRootComponent()->SetWorldRotation(FRotator(0.f, 90.0f, 0.f));
		PrisonerControllerObj->OnTaskFinished.Broadcast();
	}
	else
	{
		// UpperState : Move(2) | LowerState : Run(4)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2, 4);
		PrisonerCharacterObj->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}

const bool UUAITask_Run::HasReachedTargetPos(const FVector InChaVec, const FVector InTargetVec)
{
	//UE_LOG(LogTemp, Warning, TEXT("Cha Vec : %s"), *InChaVec.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Cha Vec : %s"), *InTargetVec.ToString());
	bool XReturnValue = false;
	bool YReturnValue = false;
	if (FMath::IsNearlyEqual(InChaVec.X, InTargetVec.X, 5.0f))
		XReturnValue = true;
	if (FMath::IsNearlyEqual(InChaVec.Y, InTargetVec.Y, 5.0f))
		YReturnValue = true;
	return (XReturnValue && YReturnValue);
}