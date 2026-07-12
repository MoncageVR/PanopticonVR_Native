#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_Anger.generated.h"

/**
 *
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_Anger : public UUAITask_Base
{
	GENERATED_BODY()

public:
	UUAITask_Anger();

protected:
	UAnimMontage* AngerMontage;

	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnAngerMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UAnimInstance* MyAnimInst;
	FVector MonPlayVec;
	FRotator MonPlayRot;
	FTimerHandle DelayGratingFlyTimer;
	class UMapObjManagerSubsystem* MapObjManagerSubSystemInst;

private:
	void CalMonPlayTransform();
	void CallHandleGratingFly();
};