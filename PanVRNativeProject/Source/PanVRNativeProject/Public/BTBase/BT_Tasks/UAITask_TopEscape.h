// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_TopEscape.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_TopEscape : public UUAITask_Base
{
	GENERATED_BODY()
	
public:
	UUAITask_TopEscape();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class AJailBuilding* mJailBuildingObj;
	bool bIsRotatePossibleFlag;
	FTimerHandle HatchOpenSuccessTimer;

private:
	void CallHandleHatchDoor();
};
