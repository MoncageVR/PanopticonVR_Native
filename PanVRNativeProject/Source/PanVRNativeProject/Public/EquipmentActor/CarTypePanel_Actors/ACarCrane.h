

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "ACarCrane.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AACarCrane : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	AACarCrane();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EquipmentRegistrable(AActor* InActor) override;

	void HandleAddCraneYaw(float InDeltaYaw);
	void HandleOperatingCraneHead();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComp")
	TObjectPtr<USceneComponent> CarCraneMainRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMeshComp")
	TObjectPtr<class UStaticMeshComponent> UFOPortal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMeshComp")
	TObjectPtr<class UStaticMeshComponent> UFOCraneHead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLUFOSubdue;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> TL_CraneDownward;

	UPROPERTY()
	TObjectPtr<UCurveFloat> CraneHeadDownwardMove_Curve;

protected:
	UFUNCTION()
	void CarCraneOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void CraneHeadOperatingTimeline(float value);

	UFUNCTION()
	void CraneHeadOperatingTimelineEnd();

private:
	FVector CraneHeadDefaultVec;

private:
	void ActuallyOperateCraneHead();
};
