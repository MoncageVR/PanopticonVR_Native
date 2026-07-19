// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_Radioactivity.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_Radioactivity : public UUAITask_Base
{
	GENERATED_BODY()

public:
	UUAITask_Radioactivity();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnExplodeMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	class AAFuelTank* FuelTankObj;
	FTimerHandle CheckExplosionForGameOverTimer;
	float RadioactivityCheckTime;
	UAnimMontage* ExplodeMontage;

private:
	void ExplosureAndGameover();
};
