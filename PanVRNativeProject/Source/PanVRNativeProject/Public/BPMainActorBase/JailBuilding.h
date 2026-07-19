// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "Core/Interface/IEquipmentInitInterface.h"
#include "JailBuilding.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API AJailBuilding : public AVRActorBase
{
	GENERATED_BODY()

public:
	AJailBuilding();
	virtual void EquipmentRegistrable(AActor* InActor) override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Call The MoveTheExitDoorSideward Handle Function, 
	void HandleExitDoor();

	// Call The MoveTheHatchSideward Handle Function, 
	void HandleHatchDoor();

#pragma region Getter
	TObjectPtr<class USplineComponent> GetSpiderManMoveRouteComp() const { return mSpiderManMoveRoute; }
	TObjectPtr<class UStaticMeshComponent> GetHatchSMComp() const { return JailHatch; }
#pragma endregion

protected:
	// Scene Component Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComponent")
	TObjectPtr<USceneComponent> JailMainRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComponent")
	TObjectPtr<USceneComponent> FirstFloorRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComponent")
	TObjectPtr<USceneComponent> SecondFloorRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComponent")
	TObjectPtr<USceneComponent> ThirdFloorRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComponent")
	TObjectPtr<USceneComponent> MoveableBuildingsRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComponent")
	TObjectPtr<USceneComponent> StaticBuildingsRoot;

	// Static Mesh Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class UStaticMeshComponent> JailExitDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class UStaticMeshComponent> JailHatch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class UStaticMeshComponent> JailRoof;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class UStaticMeshComponent> Jail1FWeaponDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class UStaticMeshComponent> Jail2FWeaponDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class UStaticMeshComponent> Jail3FWeaponDoor;

	// Collision Component Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLSubdueHatch;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USplineComponent> mSpiderManMoveRoute;

#pragma region TimelineComp
protected:
	UPROPERTY()
	TObjectPtr<class UTimelineComponent> UpwardMoveTimelineComp;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> DownwardMoveTimelineComp;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> SideWardsMoveTimelineComp;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> HatchSideWardsMoveTLComp;

	UPROPERTY()
	TObjectPtr<UCurveFloat> MoveTheWeaponDoorFloatCurve;

	UPROPERTY()
	TObjectPtr<UCurveFloat> MoveTheExitDoorFloatCurve;

	UPROPERTY()
	TObjectPtr<UCurveFloat> MoveTheHatchFloatCurve;

	UFUNCTION()
	void UpwardMoveTheDoorPlayEvent(float Value);

	UFUNCTION()
	void DownwardMoveTheDoorPlayEvent(float Value);

	UFUNCTION()
	void UpwardMoveTheDoorFinishedEvent();

	UFUNCTION()
	void DownwardMoveTheDoorFinishedEvent();

	UFUNCTION()
	void SidewardMoveTheExitDoorPlayEvent(float Value);

	UFUNCTION()
	void SidewardMoveTheExitDoorFinishedEvent();

	UFUNCTION()
	void SidewardMoveTheHatchPlayEvent(float Value);

	UFUNCTION()
	void SidewardMoveTheHatchFinishedEvent();

#pragma endregion

	// Glove ¡æ Jail : Receive Function
	UFUNCTION()
	void HandleJailReceiveByGlove();

	// AB ¡æ Jail : Receive Function
	UFUNCTION()
	void HandleJailReceiveByABButton();

	UFUNCTION()
	void HandleJailReceiveByEB(FName InTag, int32 InFloor);

private:
	TArray<TObjectPtr<UStaticMeshComponent>> SMWeaponDoorArrs;
	TArray<FVector> TargetDownVecArrs;
	TArray<FVector> TargetUpVecArrs;
	int32 CurrFloorNum = 3;
	USoundBase* GloveNJailDoorOperationSFX;
	FTransform HatchDefaultTransform;

private:
	void InitRefDoorNVector();
	// Upward Move Call Function In Use Timeline PlayFromStart
	void MoveTheDoorUpward();
	// Downward Move Call Function In Use Timeline PlayFromStart
	void MoveTheDoorDownward();
	// Sideward Move Call Function In Use Timeline PlayFromStart
	void MoveTheExitDoorSideward();

	void MoveTheHatchSideward();

	void Init_JailSplineAllPointValue();
};
