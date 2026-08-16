#include "BTBase/BT_Tasks/UAITask_Run.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_Run::UUAITask_Run()
{
	NodeName = TEXT("BTTask_Run");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UUAITask_Run::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	/*UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys)
	{
		FVector PawnLoc = PrisonerCharacterObj->GetActorLocation();
		FVector EscapeVec = PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("EscapeTargetVec"));

		FNavLocation SelfProj, TargetProj;
		bool bSelf = NavSys->ProjectPointToNavigation(PawnLoc, SelfProj, FVector(100.f, 100.f, 300.f));
		bool bTarget = NavSys->ProjectPointToNavigation(EscapeVec, TargetProj, FVector(100.f, 100.f, 300.f));

		UE_LOG(LogTemp, Warning, TEXT("[RUN] SelfProj=%d  In=%s  Out=%s"),
			(int32)bSelf, *PawnLoc.ToString(), *SelfProj.Location.ToString());
		UE_LOG(LogTemp, Warning, TEXT("[RUN] TargetProj=%d  In=%s  Out=%s  dZ=%.1f  dXY=%.1f"),
			(int32)bTarget, *EscapeVec.ToString(), *TargetProj.Location.ToString(),
			EscapeVec.Z - TargetProj.Location.Z,
			FVector::Dist2D(EscapeVec, TargetProj.Location));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[RUN] NavSys is NULL!"));
	}*/

	// UpperState : Move(2) | LowerState : Run(4)
	PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2, 4);

	if (HasReachedTargetPos(
		PrisonerCharacterObj->GetRootComponent()->GetComponentLocation(),
		PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("EscapeTargetVec"))))
	{
		PrisonerCharacterObj->GetRootComponent()->SetWorldRotation(FRotator(0.f, 90.0f, 0.f));
		PrisonerControllerObj->OnTaskFinished.Broadcast();
		//return EBTNodeResult::Succeeded;
	}
	else
	{
		// UpperState : Move(2) | LowerState : Run(4)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2, 4);
		PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}

const bool UUAITask_Run::HasReachedTargetPos(const FVector InChaVec, const FVector InTargetVec)
{
	UE_LOG(LogTemp, Warning, TEXT("Cha Vec : %s"), *InChaVec.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Cha Vec : %s"), *InTargetVec.ToString());
	bool XReturnValue = false;
	bool YReturnValue = false;
	if (FMath::IsNearlyEqual(InChaVec.X, InTargetVec.X, 5.0f))
		XReturnValue = true;

	if (FMath::IsNearlyEqual(InChaVec.Y, InTargetVec.Y, 5.0f))
		YReturnValue = true;
	UE_LOG(LogTemp, Error, TEXT("Run Move To Escape Taget Pos Reached!"))
	return (XReturnValue && YReturnValue);
}