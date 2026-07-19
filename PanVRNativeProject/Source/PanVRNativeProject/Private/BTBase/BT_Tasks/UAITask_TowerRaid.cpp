// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBase/BT_Tasks/UAITask_TowerRaid.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "BPActorBase/AElevatorButton.h"
#include "BPMainActorBase/TowerBuilding.h"
#include "CoreObj/VRGameInstance.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"
#include "Components/SplineComponent.h"

UUAITask_TowerRaid::UUAITask_TowerRaid()
{
	NodeName = TEXT("BTTask_TowerRaid");
	bCreateNodeInstance = true;
	bNotifyTick = true;

	TimeOfSplineRoute = 10.0f;
	DistanceOfSplineRoute = 0.0f;
	SpeedOfSplineRoute = 0.0f;
	CurrPositionOfSplineRoute = 0.0f;
	bIsCanMove = 0;
}

EBTNodeResult::Type UUAITask_TowerRaid::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;

	//UE_LOG(LogTemp, Log, TEXT("UAI_Task_TowerRaid Execute!!"));

	// 4 = UpperState : Dangerous , 14 = LowerState : TowerRaid
	PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(4, 14);

	UVREquipmentWorldSubsystem* TempVREquipmentSubSystemRef = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	if (!ensure(TempVREquipmentSubSystemRef)) return EBTNodeResult::Failed;

	for (IIEquipmentInitInterface* Var : TempVREquipmentSubSystemRef->GetEquipmentArr())
	{
		ElevatorButtonObj = Cast<AAElevatorButton>(Var);
		if (ElevatorButtonObj)
			break;
		else
			continue;
	}

	for (IIEquipmentInitInterface* Var : TempVREquipmentSubSystemRef->GetEquipmentArr())
	{
		TowerBuildingObj = Cast<ATowerBuilding>(Var);
		if (TowerBuildingObj)
			break;
		else
			continue;
	}

	PrisonerCharacterObj->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed")) + 10.0f;

	ElevatorButtonObj->HandleCollisionEnabled(0);
	DistanceOfSplineRoute = TowerBuildingObj->GetTowerRaidMoveRouteComp()->GetSplineLength();
	SpeedOfSplineRoute = DistanceOfSplineRoute / TimeOfSplineRoute;
	bIsCanMove = 1;

	return EBTNodeResult::InProgress;
}

void UUAITask_TowerRaid::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (bIsCanMove)
	{
		float TempPosition = DeltaSeconds * SpeedOfSplineRoute;
		CurrPositionOfSplineRoute = TempPosition + CurrPositionOfSplineRoute;

		FTransform TempTransform = 
		TowerBuildingObj->GetTowerRaidMoveRouteComp()->GetTransformAtDistanceAlongSpline(CurrPositionOfSplineRoute, ESplineCoordinateSpace::World);

		PrisonerCharacterObj->GetRootComponent()->SetRelativeTransform(
			FTransform(FRotator(90.0f, 0.0f, 180.0f), TempTransform.GetLocation(), FVector(1.0f, 1.0f, 1.0f))
		);

		if (HasReachedTargetPosition(
			PrisonerCharacterObj->GetRootComponent()->GetComponentLocation().Z, 
			TowerBuildingObj->GetTowerRaidMoveRouteComp()->GetLocationAtSplinePoint(3, ESplineCoordinateSpace::World).Z - 10.0f)
		)
		{
			bIsCanMove = 0;
			UE_LOG(LogTemp, Log, TEXT("Prisoner TowerRaid Arrive Success!"));

			// 0 = UpperState : Idle , 1 = LowerState : Default
			PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(0, 1);

			// GameOver Logic Execute Parts!
			MyVRGameMode->SetIsGameOverFlag(true);
			MyVRGameMode->GameOverCheckEvent();
		}
	}
	else
	{
		return;
	}
	return;
}

uint8 UUAITask_TowerRaid::HasReachedTargetPosition(float InPrisonerPosZ, float InTargetPosZ)
{
	if (InPrisonerPosZ >= InTargetPosZ)
		return 1;
	else
		return 0;
}
