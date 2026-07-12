#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_DoorPicking.generated.h"

/**
 *
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_DoorPicking : public UUAITask_Base
{
	GENERATED_BODY()

public:
	UUAITask_DoorPicking();

protected:
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnDoorPickingMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UAnimMontage* DoorPickingMontage;
	FVector MontagePlayVec;
	FRotator MontagePlayRot;

private:
	void CalMontagePlayTransform(float InSpawnYaw);
	int32 AdjustPrisonerUniqueNum(int32 InTempUniqueNum);
};