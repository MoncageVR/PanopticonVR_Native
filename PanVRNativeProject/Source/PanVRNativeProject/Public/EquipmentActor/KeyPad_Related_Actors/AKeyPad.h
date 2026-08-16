#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Component/UGrabComp.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "Kismet/GameplayStatics.h"
#include "AKeyPad.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAKeyPad : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()

public:
	AAKeyPad();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTimes) override;

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

	virtual void EquipmentRegistrable(AActor* InActor) override;

	// True : DownMove , False : UpMove
	void HandleKeyPadMove(uint8 InMoveFlag);

protected:
	UFUNCTION()
	void OnKeyOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnKeyOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void OnCameraOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnCameraOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

private:
#pragma region KeyRelated_Components
	// KeyInRelated : Key Main Scene Root Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SC_MainRoot;

	// KeyInRelated : StaticMesh Component Arrays (Key01 ~ Key09)
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> SM_Keys;

	// KeyInRelated : PickleKey StaticMesh Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_PickleKey;

	// KeyInRelated : BackSpaceKey StaticMesh Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_BackSpaceKey;

	// KeyInRelated : MainBody Monitor StaticMesh Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_Monitor;

	// KeyInRelated : Box Collision Component Arrays (Key01 ~ Key09)
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class UBoxComponent>> CL_Keys;

	
	// KeyInRelated : Pickle Box Collision Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CL_PickleKey;

	// KeyInRelated : BackSpace Box Collision Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CL_BackSpaceKey;
	
#pragma endregion

#pragma region KeyOutRelated_Components
	// KeyOutRelated : SceneRootComponent
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SC_ScoreBoardRoot;

	// KeyOutRelated :  ScoreBoard Bar Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_KeyPad_Bar;

	// KeyOutRelated :  ScoreBoard Body Arrays
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> SM_ScoreBoard_Bodys;

	// KeyOutRelated :  ScoreBoard Glass Arrays
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> SM_ScoreBoard_Glasses;

	// KeyOutRelated :  ScoreBoard Text Render Component Arrays
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class UTextRenderComponent>> TR_ScoreBoard_Text;
#pragma endregion

#pragma region PerisScopeRelated
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CL_PerisScope;
#pragma endregion

	UPROPERTY()
	TObjectPtr<UMotionControllerComponent> TempMCRef;

	UPROPERTY()
	TObjectPtr<class ACVRPawn> mVRPlayerPawn;

	UPROPERTY()
	TObjectPtr<class AAScopeCamera> ScopeCameraRef;

	UPROPERTY()
	TObjectPtr<USoundBase> ButtonPressSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> ReturnButtonPressSFX;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> KeyPadMoveTimelineComp;

	UPROPERTY()
	TObjectPtr<UCurveFloat> KeyPadMove_CurveF;

private:

	bool bIsOverlapping;
	UPROPERTY(VisibleAnywhere)
	TArray<uint32> FinalOutputIntArrays;
	UPROPERTY(VisibleAnywhere)
	TArray<FText> FinalOutputTextArrays;
	uint32 FinalOutputTextLength;
	uint32 CurrTextLength;
	FTimerHandle ArrayClearTimer;

	FVector TargetPos;
	bool bIsScopeFlag;

	float MaxPeekDist;

	bool  bIsInScopeView = false;
	bool  bArmed = true;
	float EnterDist = 2.0f;
	float ExitDist = 8.0f;
	float ReArmDist = 12.0f;

private:
	void ClearOutputArrays();
	void CheckOverlapColToInt(TObjectPtr<class UBoxComponent> InBoxCol);
	bool CheckOverlapColToText(uint32 InBoxNum);

	void HandleKeyPadReceiveByEmergencyButton();

	FVector GetHMDWorldLocation() const;

	UFUNCTION()
	void KeyPadMovePlayEvent(float Value);

	UFUNCTION()
	void KeyPadMoveFinishedEvent();
};
