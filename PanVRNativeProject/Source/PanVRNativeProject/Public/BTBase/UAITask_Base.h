// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UAITask_Base.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_Base : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UUAITask_Base();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//virtual void FinishLatentTask(UBehaviorTreeComponent& OwnerComp, EBTNodeResult::Type TaskResult) override;

public:
	class APrisonerController* PrisonerControllerObj;
	class APrisonerCharacter* PrisonerCharacterObj;
};
