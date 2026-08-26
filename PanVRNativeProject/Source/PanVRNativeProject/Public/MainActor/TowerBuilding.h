#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "TowerBuilding.generated.h"

class IIEquipmentInitInterface;

UCLASS()
class PANVRNATIVEPROJECT_API ATowerBuilding : public AVRActorBase
{
	GENERATED_BODY()
	
public:	
	ATowerBuilding();

	virtual void BeginPlay() override;
	// IIEquipmentInitInterface In Natural Virtual Function Overriding , For Use Delegate System 
	virtual void EquipmentRegistrable(AActor* InActor) override;
	UFUNCTION()
	void HandleSplinePointValue(int32 InFloorNum);

public:
#pragma region Getter
	FORCEINLINE int32 GetTowerCurrFloorNum() const { return ActuallyCurrFloorNum; }
	FORCEINLINE TObjectPtr<class USplineComponent> GetTowerRaidMoveRouteComp() const { return mTowerRaidMoveRoute; }
#pragma endregion

#pragma region Setter
	FORCEINLINE void SetTowerCurrFloorNum(int32 InCurrFloor) { ActuallyCurrFloorNum = InCurrFloor; }
#pragma endregion

protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> MainRoot;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> MainDesk;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> SubDesk;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> Barricade;

	UPROPERTY()
	TObjectPtr<class UBoxComponent> CLSubdueForToilet;

	UPROPERTY()
	TObjectPtr<class UAudioComponent> TBAudioPlayer;

	UPROPERTY()
	TObjectPtr<class USplineComponent> mTowerRaidMoveRoute;

protected:
	UFUNCTION()
	void HandleTowerReceiveByEB(FName InTag, int32 InFloor);

	UFUNCTION()
	void ActuallyTowerMoveCompleted();

	UFUNCTION()
	void TowerSubdueOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	int32 ActuallyCurrFloorNum = 3;
	TArray<float> TargetTowerHeights;
	FLatentActionInfo TowerLatentInfo;

	UPROPERTY()
	TObjectPtr<USoundBase> TowerMoveSFXCue;

	TArray<FVector> First_SplinePointValueArrs;
	TArray<FVector> Second_SplinePointValueArrs;
	TArray<FVector> Third_SplinePointValueArrs;

private:
	void ActuallyMoveTower(float TargetTowerHeight);
	void Init_TowerSplinePointValue();
	void Init_TowerSplineDefaultPointValue();
	void SetSplinePointValueByCurrFloorNum(int32 InTempFloorNum);
	// T(1) : Subdue On / F(0) : Subdue Off
	UFUNCTION()
	void HandleRaidSubdueReceiveByToilet(uint8 bIsSubdueFlag);
};
