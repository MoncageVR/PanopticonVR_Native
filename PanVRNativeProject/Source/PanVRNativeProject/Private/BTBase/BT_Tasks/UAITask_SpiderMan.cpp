


#include "BTBase/BT_Tasks/UAITask_SpiderMan.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "Components/SplineComponent.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "MainActor/JailBuilding.h"

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
		// UpperState : Dangerous(4) | LowerState : TowerRaid(13)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(4, 13);
		PrisonerCharacterObj->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		bIsCanMoveAlongSpline = 1;

		return EBTNodeResult::InProgress;
	}
	else
	{
		// UpperState : Move(2) | LowerState : Run(4)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2, 4);
		PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));

		UVREquipmentWorldSubsystem* TempVREquipmentSubSystem = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
		if (!ensure(TempVREquipmentSubSystem)) return EBTNodeResult::Failed;

		for (TScriptInterface<IIEquipmentInitInterface> Equip : TempVREquipmentSubSystem->GetEquipmentArr())
		{
			IIEquipmentInitInterface* IEquipPtr = Equip.GetInterface();
			JailBuildingObj = Cast<AJailBuilding>(IEquipPtr);
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
