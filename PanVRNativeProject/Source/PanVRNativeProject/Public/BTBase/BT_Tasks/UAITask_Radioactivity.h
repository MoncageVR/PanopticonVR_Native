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
	UPROPERTY()
	TObjectPtr<class AAFuelTank> FuelTankObj;

	UPROPERTY()
	TObjectPtr<UAnimMontage> ExplodeMontage;

	UPROPERTY()
	TObjectPtr<USoundBase> SFX_Explode;

	FTimerHandle CheckExplosionForGameOverTimer;
	float RadioactivityCheckTime;

private:
	void ExplosureAndGameover();
};
