// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBase/BT_Tasks/UAITask_SpiderMan.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "Components/SplineComponent.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"
#include "BPMainActorBase/JailBuilding.h"

UUAITask_SpiderMan::UUAITask_SpiderMan()
{
	NodeName = TEXT("BTTask_SpiderMan");
	bCreateNodeInstance = true;
	bNotifyTick = true;

	bIsCanMoveAlongSpline = 0;
	TimeOfSplineRoute_InSpiderManState = 10.0f;
}

EBTNodeResult::Type UUAITask_SpiderMan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;
	//UE_LOG(LogTemp, Log, TEXT("UAI_Task SpiderMan Execute!!"));

	if (HasReachedCeilingEscapeTargetVec(PrisonerCharacterObj->GetRootComponent()->GetComponentLocation(),
		PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("CeilingEscapeTargetVec"))))
	{
		//UE_LOG(LogTemp, Log, TEXT("SpiderMan State Is Run Target Vec Arrived"));
		// 4 = UpperState : Dangerous , 14 = LowerState : TowerRaid
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(4, 14);
		PrisonerCharacterObj->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		bIsCanMoveAlongSpline = 1;

		return EBTNodeResult::InProgress;
	}
	else
	{
		// 2 = UpperState : Move , 5 = LowerState : Run
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2, 5);
		PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));

		UVREquipmentWorldSubsystem* TempVREquipmentSubSystem = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
		if (!ensure(TempVREquipmentSubSystem)) return EBTNodeResult::Failed;
		for (IIEquipmentInitInterface* Var : TempVREquipmentSubSystem->GetEquipmentArr())
		{
			JailBuildingObj = Cast<AJailBuilding>(Var);
			if (JailBuildingObj)
				break;
			else
				continue;
		}

		DistanceOfSplineRoute_InSpiderManState = JailBuildingObj->GetSpiderManMoveRouteComp()->GetSplineLength();
		SpeedOfSplineRoute_InSpiderManState = DistanceOfSplineRoute_InSpiderManState / TimeOfSplineRoute_InSpiderManState;

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}

void UUAITask_SpiderMan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (bIsCanMoveAlongSpline)
	{
		//UE_LOG(LogTemp, Log, TEXT("SpiderMan Wall Climbing~"));
		//JailBuildingObj->GetSpiderManMoveRouteComp();

		float TempPos = DeltaSeconds * SpeedOfSplineRoute_InSpiderManState;
		CurrPositionOfSplineRoute_InSpiderManState = TempPos + CurrPositionOfSplineRoute_InSpiderManState;

		FTransform TempTransform = JailBuildingObj->GetSpiderManMoveRouteComp()->GetTransformAtDistanceAlongSpline(CurrPositionOfSplineRoute_InSpiderManState, ESplineCoordinateSpace::World, false);

		PrisonerCharacterObj->GetRootComponent()->SetRelativeLocationAndRotation(TempTransform.GetLocation(), TempTransform.GetRotation());

		if (HasReachedTopEscapeTargetVec(
			PrisonerCharacterObj->GetRootComponent()->GetComponentLocation(),
			JailBuildingObj->GetSpiderManMoveRouteComp()->GetLocationAtSplinePoint(6, ESplineCoordinateSpace::World)
		))
		{
			//UE_LOG(LogTemp, Log, TEXT("TopEscape Vec Arrived"));
			PrisonerControllerObj->OnTaskFinished.Broadcast();
			bIsCanMoveAlongSpline = 0;
			PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(0, 1);
		}
	}
	else
	{
		return;
	}

	return;
}

const bool UUAITask_SpiderMan::HasReachedCeilingEscapeTargetVec(const FVector InChaVec, const FVector InTargetVec)
{
	bool XReturnValue, YReturnValue = false;
	if (FMath::IsNearlyEqual(InChaVec.X, InTargetVec.X, 5.0f))
		XReturnValue = true;
	if (FMath::IsNearlyEqual(InChaVec.Y, InTargetVec.Y, 5.0f))
		YReturnValue = true;

	return (XReturnValue && YReturnValue);
}

const bool UUAITask_SpiderMan::HasReachedTopEscapeTargetVec(const FVector InChaVec, const FVector InTopEscapeTargetVec)
{
	return InChaVec.Equals(InTopEscapeTargetVec, 5.0f);
}
