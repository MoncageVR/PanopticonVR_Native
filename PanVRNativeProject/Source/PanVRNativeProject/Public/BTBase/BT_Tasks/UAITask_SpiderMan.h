// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_SpiderMan.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_SpiderMan : public UUAITask_Base
{
	GENERATED_BODY()

public:
	UUAITask_SpiderMan();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	uint8 bIsCanMoveAlongSpline;

	float TimeOfSplineRoute_InSpiderManState;
	float DistanceOfSplineRoute_InSpiderManState;
	float SpeedOfSplineRoute_InSpiderManState;
	float CurrPositionOfSplineRoute_InSpiderManState;

	class AJailBuilding* JailBuildingObj;

private:
	const bool HasReachedCeilingEscapeTargetVec(const FVector InChaVec, const FVector InTargetVec);

	const bool HasReachedTopEscapeTargetVec(const FVector InChaVec, const FVector InTopEscapeTargetVec);
};
