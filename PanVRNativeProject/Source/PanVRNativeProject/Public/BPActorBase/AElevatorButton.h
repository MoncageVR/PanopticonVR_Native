// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Component/UGrabComp.h"
#include "Core/Interface/IGrabInterface.h"
#include "AElevatorButton.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAElevatorButton : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()

#pragma region BasicFunction
public:
	AAElevatorButton();

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// IIEquipmentInitInterface In Natural Virtual Function Overriding
	// For Use Delegate System 
	virtual void EquipmentRegistrable(AActor* InActor) override;

	void HandleCollisionEnabled(uint8 bIsColActivated);
#pragma endregion

public:
#pragma region Getter
	int32 GetCurrFloorNum() const;
	int32 GetPressedFloorNum() const;
#pragma endregion

#pragma region Setter
	void SetCurrFloorNum(int32 InCurrFloor);
	void SetPressedFloorNum(int32 InPressFloor);
#pragma endregion

	UFUNCTION(BlueprintCallable)
	void Debug_TowerTo_1stFloor();

	UFUNCTION(BlueprintCallable)
	void Debug_TowerTo_2ndFloor();

	UFUNCTION(BlueprintCallable)
	void Debug_TowerTo_3rdFloor();

protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComp")
	TObjectPtr<USceneComponent> EBMainRoot;

	// StaticMesh Elevator Button 1 Floor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMeshComp")
	TObjectPtr<UStaticMeshComponent> EBB1F;

	// StaticMesh Elevator Button 2 Floor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMeshComp")
	TObjectPtr<UStaticMeshComponent> EBB2F;

	// StaticMesh Elevator Button 3 Floor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMeshComp")
	TObjectPtr<UStaticMeshComponent> EBB3F;

	// Spline Component For Grabbing Elevator Button Body Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SplineComp")
	TObjectPtr<class USplineComponent> EBMoveRail;
#pragma endregion

#pragma region Collision
	// Box Collision Component Elevator Button 1 Floor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|CollisionComp")
	TObjectPtr<class UBoxComponent> CLB1F;

	// Box Collision Component Elevator Button 2 Floor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|CollisionComp")
	TObjectPtr<class UBoxComponent> CLB2F;

	// Box Collision Component Elevator Button 3 Floor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|CollisionComp")
	TObjectPtr<class UBoxComponent> CLB3F;
#pragma endregion

#pragma region CollisionFunc
	UFUNCTION()
	void EBBOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void EBBFOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
#pragma endregion

	// After Moving to Default Position , EB Setting Function To enable-grabbing
	UFUNCTION()
	void UpdatePullingBackMoveCompleted();

private:
	// Motion Controller Reference Variable
	class UMotionControllerComponent* TempMCRef = nullptr;
	// AVRHand Reference Variable
	class AVRHand* TempHand = nullptr;

	// EB Box Collision is Overlapping Check Flag Variable
	uint32 bIsOverlapping = 0;

	// After overlap End, to give a delay of 1 second Related Timer Variable
	FTimerHandle OverlapDelyTimer;
	// EB On Grabbed Related Timer Variable
	FTimerHandle EBMoveTimer;

	// Current Floor Num Variable
	int32 CurrFloor = 3;
	// Pressed By Player Floor Num Variable
	int32 PressedFloorNum = 0;

	// Pull And Button Press Related Sound Variable
	USoundBase* PullSFX;
	USoundBase* ButtonPressSFX;

	class ATowerBuilding* TowerBuildingRef;

private:
	// After overlap End, to give a delay of 1 second.
	// Using : OverlapDelayTimer
	void OverlapIndexDelay();
	// After Grab, EB Move Function using MC
	// Using : EBMoveTimer
	void UpdatePullingMove();
	// After Drop, EB Move Function to Default Position
	void UpdatePullingBackMoving();

	void SetEBColEnabled(uint8 InOperateFlag);

	// Jail ¡æ EB : Receive Function
	void HandleEBReceiveByJail(uint8 InEBControlFlag);

};
