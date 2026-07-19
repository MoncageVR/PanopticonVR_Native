// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_Floating.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_Floating : public UUAITask_Base
{
	GENERATED_BODY()
	
public:
	UUAITask_Floating();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnFloatingFinishedFunc();

private:
	FVector FloatingTargetVec;
	FLatentActionInfo FloatingStateLatentInfo;
	float FloatingExecutionTime;

private:
	void ActuallyMoveToTargetVec();

};
