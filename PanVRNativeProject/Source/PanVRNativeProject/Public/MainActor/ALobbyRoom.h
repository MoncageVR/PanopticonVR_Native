#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Component/UGrabComp.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "ALobbyRoom.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AALobbyRoom : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()

public:
	AALobbyRoom();
	virtual void BeginPlay() override;

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

	UFUNCTION()
	void HandleTutoEvent(int32 InEventID);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SC_MainRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_SpaceEV;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_Cup;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_RoomWall;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_RoomStuff;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_StartLeverHandle;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_SL_Roller01;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_SL_Roller02;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_SL_Roller03;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> CL_Handle;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CL_TapeTarget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UTimelineComponent> TL_TapeMove;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> LobbyMonitor_Glasses;


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_TapeHintArrow;

protected:

	UFUNCTION()
	void TapePathOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void TapePathOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void TapeMovePlayEvent(float Value);

	UFUNCTION()
	void TapeMoveFinishedEvent();

private:
	
	
private:
	FRotator AtFirstHandleRot;
	FTimerHandle StartLeverMoveTimer;
	uint8 bIsTapeMoveingFlag;

	UPROPERTY()
	UMotionControllerComponent* TempMCRef;

	UPROPERTY()
	TObjectPtr<UCurveFloat> CF_TapeMoveIn;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AATape> NewTape;

	UPROPERTY()
	TObjectPtr<class ULevelSequence> LQ_Roller; // LevelSequence Variable To Play

	UPROPERTY()
	TSoftObjectPtr<class ULevelSequence> LQ_TapeOut;

	UPROPERTY()
	TObjectPtr<class UVRDialogueManagerSubsystem> TempDialogueMgr;

	UPROPERTY()
	TObjectPtr<UMaterialInstance> MI_BossMonitor;

	UPROPERTY()
	TObjectPtr<UMaterial> M_Hologram;

private:
	void AdjustVecNRot(UMotionControllerComponent* InMC);

	void UpdateStartLever();

	void LeverOnGameStartEvent();

	UFUNCTION()
	void StartTapeOutLQInLobbyRoom(); // Lobby Romm In Level Sequence Start Function

	UFUNCTION()
	void StartRollerLQLobbyRoom();

	UFUNCTION()
	void OnLobbyRoomTapeOutLQDone(); // LobbyRoom In Level Sequence End After CallBack Receive Function

	UFUNCTION()
	void OnLobbyRoomRollerLQDone();

	void SwitchScreenMode(bool InScreenMode); // T : Jack Mode , F : Default Mode

	void SwitchGameStartLeverStatus(bool InStatusFlag); // T : Enabled , F : Disabled

	void ArrowPointingTotheTapeShelf();
};
