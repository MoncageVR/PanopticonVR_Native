#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_AssistNeighbor.generated.h"

USTRUCT(BlueprintType)
struct FNeighborInfo
{
	GENERATED_BODY()

public:
	FNeighborInfo();
	FNeighborInfo(FVector InVec, int32 InNum, FRotator InRot) : TargetPosition(InVec), TargetUniqueNum(InNum), TargetRotation(InRot) {};

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetUniqueNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator TargetRotation;
};

/**
 *
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_AssistNeighbor : public UUAITask_Base
{
	GENERATED_BODY()

public:
	UUAITask_AssistNeighbor();

protected:
	UPROPERTY()
	FNeighborInfo TargetStructureInfo;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UAnimMontage* NeighborDoorPickingMontage;

private:
	void AdjustTargetPrisonerUniqueNum(int32 InMyUniqueNum);
	FNeighborInfo AdjustTargetMovePos(int32 InTargetPrisonerNum);

	const bool HasReachedNeighborTargetVec(const FVector InChaVec, const FVector InTargetVec);
};