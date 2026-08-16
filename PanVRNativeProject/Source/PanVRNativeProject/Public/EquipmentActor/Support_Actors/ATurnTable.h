#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "CoreCommon/Component/UGrabComp.h"
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
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> TTMainRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> TTGlass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> TTHandleLever;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> LeverStandardSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> TTLeftButton;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> TTCLLB;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> TTRightButton;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> TTCLRB;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CLLPTarget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CLLPSpawn;

	UPROPERTY(VisibleAnywhere)
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
	UPROPERTY(VisibleAnywhere)
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

	//TArray<UMaterialInstance*> TTGlassMats;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> TTGlassMID;

	UPROPERTY()
	TObjectPtr<UMaterialInstance> GlassBaseMat;

	UPROPERTY()
	TArray<TObjectPtr<UTexture>> TTGlassTextures;

	uint32 bIsAlreadySpawnLP = 0;
	uint32 bIsLPSpawnOperation = 0;

protected:
	UPROPERTY()
	TObjectPtr<USoundBase> SFXLPDropSound;
	UPROPERTY()
	TObjectPtr<USoundBase> SFXButtonPressSound;
	UPROPERTY()
	TObjectPtr<USoundBase> SFXLeverPullSound;

	UPROPERTY()
	TObjectPtr<USoundBase> SCSfxTTBGMCues;

	UFUNCTION()
	void PlaySoundBGM(int TempCueNum);

	UPROPERTY(VisibleAnywhere)
	bool bIsGameStarted = 0;

public:
	UFUNCTION()
	void HandleTTReceiveByGTW(bool TempGameStartFlag);
};
