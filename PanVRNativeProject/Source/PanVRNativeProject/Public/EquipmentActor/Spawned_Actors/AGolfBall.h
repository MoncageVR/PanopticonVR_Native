

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AGolfBall.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAGolfBall : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	AAGolfBall();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void UpdateBallTrajectorySpline(FVector InCurveStartVec, FVector InCurveEndVec);

	UFUNCTION()
	void SetGolfEffectEmergenceDegree(float InMinDegree, float InMaxDegree, float InHeight);

protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> SC_BallMainRoot;

	UPROPERTY()
	TObjectPtr<class USphereComponent> CL_Ball;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USplineComponent> SP_BallTrajectoryRoute;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> TL_BallTrajectory;

	UPROPERTY()
	TObjectPtr<class UCurveFloat> CF_BallTrajectory;

protected:
	UFUNCTION()
	void BallCLOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void BallFlyPlayEvent(float InFlyTimeValue);

	UFUNCTION()
	void BallFlyFinishedEvent();

private:
	float SpawnTheta;
	float SpawnRadius;
	float SpawnGenerationPosZ;
};
