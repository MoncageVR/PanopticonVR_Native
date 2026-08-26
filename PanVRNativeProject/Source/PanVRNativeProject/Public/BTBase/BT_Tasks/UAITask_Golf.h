

#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_Golf.generated.h"


class AAGolfClub;

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_Golf : public UUAITask_Base
{
	GENERATED_BODY()
	
public:
	UUAITask_Golf();

protected:
	UPROPERTY()
	TObjectPtr<class AAGolfClub> MyGolfClubObj;

	UPROPERTY()
	TObjectPtr<class AAGolfBall> MyGolfBallObj;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
	TObjectPtr<USoundBase> SFX_GolfShooting;

	uint8 GolfSwingCount;
	FVector GolfBallTargetVec;
	FVector GolfBallStartAndSpawnVec;

	float GolfBallTargetMinDegree;
	float GolfBallTargetMaxDegree;

	FTimerHandle GolfBallMakeTimer;
	FTimerHandle GolfStateNextForWaitTimer;

	float GolfBallMake_Time = 1.2f;
	float GolfNextActionWait_Time = 4.63f;

private:
	UFUNCTION()
	void GolfBallFly();

	// GolfBall Start Vector , End Vector , Spawn Transform , Spawn Parameters Initialize, After Spawn And GolfBall Trajectory Update And Launch Recursion Function
	UFUNCTION()
	void ActuallyGolfBallFly();

	// Places the actor at the specified position and rotation before entering the state.
	UFUNCTION()
	void SetStateEntryTransform();

	// Spawns the golf club actor, attaches it to the character's socket, and sets the attachment-complete flag.
	UFUNCTION()
	void SpawnAndAttachGolfClub();

	// A function that determines the prisoner number that played golf, and determines the destination point and the angle to the destination point.
	UFUNCTION()
	void DetermineTheinCurveEnd(int32 InmUniqueNum);

	// Target Tower Barricade Height DeterMine Function
	UFUNCTION()
	float DeterminetheTowerHeight();

	// Golf State End Function : GolfClubObject Destroy , Animation Idle(Default) Change , NextTask Passing
	UFUNCTION()
	void GolfStateEnd();
};
