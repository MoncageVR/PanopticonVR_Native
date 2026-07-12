#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AGrating.generated.h"

/**
 *
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAGrating : public AVRActorBase
{
	GENERATED_BODY()

public:
	AAGrating();
	virtual void EquipmentRegistrable(AActor* InActor) override;
	virtual void BeginPlay() override;

	void GratingOpen();
	void GratingFly(int32 InIndex);

public:
#pragma region Getter
	FORCEINLINE int32 GetIndividualNum() const { return IndividualNum; }
#pragma endregion
#pragma region Setter
	void SetIndividualNum(int32 InNum) { IndividualNum = InNum; }
#pragma endregion

protected:
	UPROPERTY()
	FTimerHandle DelayTimerHandle;

	UPROPERTY()
	FTimerHandle ReGenerationTimerHandle;

	UPROPERTY()
	FTimerHandle GravityDisableTimer;

	UPROPERTY()
	FTimerHandle FlyingStopTimer;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MainSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> StaticMeshSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UTimelineComponent> mUpTimelineComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UTimelineComponent> mDownTimelineComp;

protected:
	UFUNCTION()
	void GratingOpenPlayEvent(float Value);

	UFUNCTION()
	void GratingClosePlayEvent(float Value);

	UFUNCTION()
	void GratingOpenFinishedEvent();

	UFUNCTION()
	void GratingCloseFinishedEvent();

private:
	int32 IndividualNum = 0;
	FVector FlyDirection;

	UCurveFloat* UpTheGratingFloatCurve;
	UCurveFloat* DownTheGratingFloatCurve;

	uint8 bIsGratingOpen;
	uint8 bIsGratingNotInPlace;

private:
	void GratingOpenStart(); // Grating Up Call Function In Use Timeline PlayFromStart
	void GratingCloseStart(); // Grating Down Call Function In Use Timeline PlayFromStart

	void GratingFlyStop();
	void GratingGravityDisable();
	void ReGenerationGrating();
};
