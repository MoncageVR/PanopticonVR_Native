#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "CoreCommon/Interface/IEquipmentInitInterface.h"
#include "JailBuilding.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API AJailBuilding : public AVRActorBase
{
	GENERATED_BODY()

public:
	AJailBuilding();
	virtual void EquipmentRegistrable(AActor* InActor) override;
	virtual void BeginPlay() override;

	// Call The MoveTheExitDoorSideward Handle Function, 
	UFUNCTION()
	void HandleExitDoor();

	// Call The MoveTheHatchSideward Handle Function, 
	UFUNCTION()
	void HandleHatchDoor();

#pragma region Getter
	TObjectPtr<class USplineComponent> GetSpiderManMoveRouteComp() const { return mSpiderManMoveRoute; }
	TObjectPtr<class UStaticMeshComponent> GetHatchSMComp() const { return JailHatch; }
#pragma endregion

protected:
	// Scene Component Variables
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> JailMainRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> FirstFloorRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SecondFloorRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> ThirdFloorRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveableBuildingsRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> StaticBuildingsRoot;

	// Static Mesh Variables
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> JailExitDoor;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> JailHatch;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> JailRoof;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> Jail1FWeaponDoor;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> Jail2FWeaponDoor;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> Jail3FWeaponDoor;

	// Collision Component Variable
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CLSubdueHatch;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USplineComponent> mSpiderManMoveRoute;

#pragma region TimelineComp
protected:
	UPROPERTY()
	TObjectPtr<class UTimelineComponent> UpwardMoveTimelineComp;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> DownwardMoveTimelineComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UTimelineComponent> SideWardsMoveTimelineComp;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> HatchSideWardsMoveTLComp;

	UPROPERTY()
	TObjectPtr<UCurveFloat> MoveTheWeaponDoorFloatCurve;

	UPROPERTY(VisibleAnywhere)
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

	// Glove ¡æ Jail : Receive Function : GloveDoor DownMove(Close) Request
	UFUNCTION()
	void HandleJailReceiveByGlove();

	// AB ¡æ Jail : Receive Function : 
	// Case1 : InTargetName == Glove : GloveDoor UpMove(Open) Request
	// Case2 : InTargetName == TopEscape : TopEscape State Prisoner Subdue Request
	UFUNCTION()
	void HandleJailReceiveByABButton(FName InTargetName);

	UFUNCTION()
	void HandleJailReceiveByEB(FName InTag, int32 InFloor);

	// Begin Overlap For Collision Component Hatch
	UFUNCTION()
	void OverlapHatchBoxBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> SMWeaponDoorArrs;
	TArray<FVector> TargetDownVecArrs;
	TArray<FVector> TargetUpVecArrs;
	int32 CurrFloorNum = 3;

	UPROPERTY()
	TObjectPtr<USoundBase> GloveNJailDoorOperationSFX;
	FTransform HatchDefaultTransform;

	UPROPERTY()
	TObjectPtr<class AVRGameMode> MyVRGM;

	UPROPERTY()
	TObjectPtr<USoundBase> SFX_ExitDoor;

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
