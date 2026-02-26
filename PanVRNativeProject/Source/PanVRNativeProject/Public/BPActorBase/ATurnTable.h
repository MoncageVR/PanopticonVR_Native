// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Interface/IGrabInterface.h"
#include "Core/Component/UGrabComp.h"
#include "ATurnTable.generated.h"

class AALP;

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AATurnTable : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()

public:
	AATurnTable();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeletaTime) override;

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

	virtual void EquipmentRegistrable(AActor* InActor) override;

protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComp")
	TObjectPtr<USceneComponent> TTMainRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> TTGlass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> TTHandleLever;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComp")
	TObjectPtr<USceneComponent> LeverStandardSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> TTLeftButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> TTCLLB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> TTRightButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> TTCLRB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLLPTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLLPSpawn;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Audio")
	TObjectPtr<class UAudioComponent> TTSCAudioPlayer;

#pragma endregion


#pragma region CollisionFunc
	// Left Button Collision Functions
	UFUNCTION()
	void OverlapLBBoxBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OverlapLBBoxEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	// Right Button Collision Functions
	UFUNCTION()
	void OverlapRBBoxBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OverlapRBBoxEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	// Attahc to LP Collision Functions
	UFUNCTION()
	void OverlapLPBoxBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OverlapLPBoxEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	// Spawn LP Check Collision Functions
	UFUNCTION()
	void OverlapSpawnLPBoxEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

#pragma endregion

protected:
	UPROPERTY()
	TObjectPtr<AALP> AttachingLP;

	UPROPERTY()
	TObjectPtr<AALP> NewSpawnLP;

protected:
	UFUNCTION()
	void SpawnDefaultLP(FVector SpawnPos);

	UFUNCTION()
	void AdjustGCPosNRotByGrabbed();

	// TurnTable Handle Lever Movement Related Timer Call Function
	UFUNCTION()
	void OperateTTLeverMovement();

	// Spawn New LP Movement Related Timer Call Function
	UFUNCTION()
	void UpdateNewSpawnLPLocation();
	
	// In Reality New LP Spawn Function
	UFUNCTION()
	void SpawnNewLP();

	UFUNCTION()
	void OperateLPRotationInPlace();

private:
	class UMotionControllerComponent* TempMCRef;

	FTimerHandle TTLeverMovementTimer;
	FTimerHandle LPMovementTimer;
	FTimerHandle LPRotationInPlaceTimer;

	int32 CurrSongNum = 0;
	int32 PlaySongNum = 0;
	uint32 bIsTouching = 0;

	TArray<UMaterialInstance*> TTGlassMats;

	uint32 bIsAlreadySpawnLP = 0;
	uint32 bIsLPSpawnOperation = 0;

protected:
	USoundBase* SFXLPDropSound;
	USoundBase* SFXButtonPressSound;
	USoundBase* SFXLeverPullSound;

	USoundBase* SCSfxTTBGMCues;

	//UFUNCTION()
	//void PlaySoundEffect(USoundBase* TempSFXSound, FVector TempSoundPlayPos);

	UFUNCTION()
	void PlaySoundBGM(int TempCueNum);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VALUE|Flag")
	bool bIsGameStarted = 0;

public:
	UFUNCTION()
	void GameStartAfterPlaySoundBGM(bool TempGameStartFlag);
};
