// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_TowerRaid.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_TowerRaid : public UUAITask_Base
{
	GENERATED_BODY()

public:
	UUAITask_TowerRaid();

protected:
	//UPROPERTY()
	//TObjectPtr<UVREquipmentWorldSubsystem> TempVREquipmentSubSystemRef;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class AAElevatorButton* ElevatorButtonObj;
	class ATowerBuilding* TowerBuildingObj;

	float TimeOfSplineRoute;
	float DistanceOfSplineRoute;
	float SpeedOfSplineRoute;
	float CurrPositionOfSplineRoute;

	uint8 bIsCanMove;

private:
	uint8 HasReachedTargetPosition(float InPrisonerPosZ, float InTargetPosZ);
};
