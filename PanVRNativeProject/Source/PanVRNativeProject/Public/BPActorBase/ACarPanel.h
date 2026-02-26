// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Interface/IGrabInterface.h"
#include "Components/TimelineComponent.h"
#include "ACarPanel.generated.h"


/*
ACraPanel Object Hierarchy

Variable Name ( Object Role - Component Name )

CarMainRoot
	- ActorBaseMesh( Main Handle - Static Mesh Component )
		- GC( Main Hanlde GC - Grab Component )
		- MainHandleStandardRoot( Main Handle Rotate Standard - Scene Component )

	- CarMainBody(Car Body - Static Mesh Component)
		- CarSubHandle( Sub Handle - Static Mesh Component )
			- SubHandleGC( Sub Handle GC - Grab Component )
			- SubHandleStandardRoot( Sub Handle Rotate Standard - Scene Component )
			- CLSubHandle( Sub Handle Collision - Capsule Component )
		- CarLidDoor( Lid Door - Static Mesh Component )
			- LidDoorGC( Lid Door GC - Grab Component )
		- Carkey( CarKey - Static Mesh Component )
			- CarKeyGC( CarKey GC - Grab Component )
			- CLCarKey( CarKey Collision - Box Component )
		- KeyHole( KeyHole - Static Mesh Component )
			- CLKeyHole( KeyHole Collision - Box Component )
		- KeyHoleBody( Body Of KeyHole - Static Mesh Component )
		- CarPanelGlass1( Glass - Static Mesh Component )
		- CarPanelGlass2( Glass - Static Mesh Component )
*/
UCLASS()
class PANVRNATIVEPROJECT_API AACarPanel : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()
	
public:
// Constructor , BeginPlay , Tick Function etc of Basic Function
#pragma region BasicFunc
	AACarPanel();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
#pragma endregion

// GrabInterface Function override Part
#pragma region IIGrabInterFace
	// IGrabInterface Function override
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;
#pragma endregion

protected:
// Actor Hierarchy in SceneComponents
#pragma region SceneComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComp")
	TObjectPtr<class USceneComponent> CarMainRoot;

	// Car Main Handle Related Components Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComp")
	TObjectPtr<class USceneComponent> MainHandleStandardRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComp")
	TObjectPtr<class USceneComponent> SubHandleStandardRoot;
#pragma endregion

// Actor Hierarchy in Static Mesh Components
#pragma region StaticMeshComponent
	// Car Main Body Related Components Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> CarMainBody;

	// Car Sub Handle Related Components Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> CarSubHandle;

	// Car Lid Door Related Components Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> CarLidDoor;

	// CarKey Spawn Box Related Components Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> CarKey;

	// KeyHole Related Components Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> KeyHole;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> KeyHoleBody;

	// Glass Related Components Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> CarPanelGlass1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> CarPanelGlass2;
#pragma endregion

// Actor Hierarchy in GrabComponents
#pragma region GrabComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|GC")
	TObjectPtr<class UUGrabComp> SubHandleGC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|GC")
	TObjectPtr<UUGrabComp> LidDoorGC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|GC")
	TObjectPtr<class UUGrabComp> CarKeyGC;
#pragma endregion

// Actor Hierarchy in Collision Components
#pragma region Collision Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UCapsuleComponent> CLSubHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLCarKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLKeyHole;
#pragma endregion


#pragma region MainHandleFunc
	// Main Handle Grabbed Actually Operating Fucntion Part
	UFUNCTION()
	void MainHandleOperating(); 
#pragma endregion

#pragma region SubHandleFunc
	// Sub Handle Grabbed Actually Operating Fucntion Part
	UFUNCTION()
	void SubHandleOperating();
#pragma endregion

// KeyHole And CarKey Attaching After Operating CarPanel Execution Part Related Function
#pragma region CarKeyRelatedFunc
protected:
	//
	UFUNCTION()
	void KeyHoleOperatingTimeline(float value);

	//
	UFUNCTION()
	void KeyHoleOperatingTimelineEnd();

	//
	UFUNCTION()
	void KeyHoleOperatingEnd();

	UFUNCTION()
	void OverlapBeginByCarKey(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OverlapEndByCarkey(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

#pragma endregion

#pragma region CarKeyRelatedVariables
	// Spawned CarKey
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class AACarKey> NewSpawnCarKey; 

	// KeyHole Attached CarKey
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class AACarKey> AttachedCarKey;

	//
	UTimelineComponent* CarPanelTimeline;

	// 
	UCurveFloat* KeyHoleMoveCurve;
#pragma endregion

private:
#pragma region CoordinateVars
	FVector AtFirstMainHandleGCVec; // Main Handle Gc First Local Pos
	FVector AtFirstSubHandleGCVec; // Sub Handle Gc First Local Pos
	FRotator AfFirstSubHandleRot; // Sub Handle SM First Local Rot
#pragma endregion

#pragma region CarKeyFlagVar
	uint8 bIsCarKeyAttaching = 0;
	bool bIsTurnOnCar = false;
	uint8 bIsCoolDown = 0;
#pragma endregion

#pragma region CarMainBodyShaker
	bool MainBodyShakingFlag = false;
	FRotator ShakeRot; // 
	float ShakingLevel = 0.5f; // Car Main Body Operate = Shaking Level Variable
#pragma endregion

#pragma region TimerHandle
	FTimerHandle SubHandleOperateTimer; // Sub Handle Operate Timer Handler
	FTimerHandle DelayTimer; // CarKey Operate After Delay Timer Handler
	FTimerHandle MainHandleOperateTimer; // Main Handle Rotate Operation Timer Handler
#pragma endregion

#pragma region MainHandleVar
	FVector PrevVec;

#pragma endregion
};
