// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_Subdue.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_Subdue : public UUAITask_Base
{
	GENERATED_BODY()

public:
	UUAITask_Subdue();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FTimerHandle SimulDelayTimer;
	FTimerHandle HideMeshDelayTimer;
	FTimerHandle ReSpawnDelayTimer;

private:
	void thisMeshSimulatingFunc();
	void BeginReSpawnRagdool();
	void RegenerationFromRagdoll();

};
