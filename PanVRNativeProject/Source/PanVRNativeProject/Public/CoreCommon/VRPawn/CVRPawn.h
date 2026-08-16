#pragma once

#include "CoreMinimal.h"
#include "CoreCommon/VRPawn/VRCharacterBase.h"
#include "CVRPawn.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API ACVRPawn : public AVRCharacterBase
{
	GENERATED_BODY()

public:
	ACVRPawn();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTimes) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayerMovingUpAndDownInStage(uint8 InDir);

	UFUNCTION()
	void HandleMovePlayerToFloor(FName InTag, int32 InTargetFloor); // Function Call By ElevatorButton

	UFUNCTION()
	void HandleDownMovePlayer(); // Call GameOver 

	void HandleMaskOpacity(float OpacityValue);

#pragma region Getter
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetHMDSMComp() const { return HMD; }
#pragma endregion

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> ChairPlatform;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ChairBody;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> ChairTowerHead; 

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> HMD;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_MaskPlane;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialInstance> MaskMI;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaskMID_0Index;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaskMID_1Index;

	UPROPERTY()
	class UInputMappingContext* IMC_Default;

	UPROPERTY()
	class UInputMappingContext* IMC_Hands;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> VRPawnUpMovementTimeline;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> VRPawnDownMovementTimeline;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> TL_VRPawnDownMoveInLobby;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> TL_VRPawnUpMoveInLobby;

protected:
	UFUNCTION()
	void VRPawnMoveUpTLFunc(float Value);

	UFUNCTION()
	void VRPawnMoveUpTLEndFunc();

	UFUNCTION()
	void VRPawnMoveDownTLFunc(float Value);

	UFUNCTION()
	void VRPawnMoveDownTLEndFunc();

	UFUNCTION()
	void VRPawnDownMoveInLobbyTLFunc(float Value);

	UFUNCTION()
	void VRPawnDownMoveInLobbyTLEndFunc();

	UFUNCTION()
	void VRPawnUpMoveInLobbyTLFunc(float Value);

	UFUNCTION()
	void VRPawnUpMoveInLobbyTLEndFunc();

private:
	UPROPERTY()
	TSubclassOf<class AVRHand> LeftHandBPClass;

	UPROPERTY()
	TSubclassOf<class AVRHand> RightHandBPClass;

	UPROPERTY()
	TObjectPtr<UCurveFloat> VRPawnMoveUpCurve;

	UPROPERTY()
	TObjectPtr<UCurveFloat> VRPawnLobbyDownMoveCurve;

	UPROPERTY()
	TObjectPtr<UCurveFloat> VRPawnLobbyUpMoveCurve;

	UPROPERTY()
	TSubclassOf<class UVRPawnHUD> VRPawnHUDWidgetClass;

	UPROPERTY()
	TObjectPtr<class UVRPawnHUD> HUDWidgetInstance;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWidgetComponent> HUDWidgetComp;

	UPROPERTY()
	TObjectPtr<class AVRLobbyGameMode> mVRLobbyGMRef;



	int32 CurrFloorNum;
	int32 PressedFloorNum;
	TArray<float> TargetPlayerHeights;

private:
	void InitFloorData();

	void HideTowerHeadMesh(bool bIsHideFlag);

	void GameStartInLobbyEvent();

	UFUNCTION()
	void HandleVRPawnReceivceByGTW(bool InFlag);

	UFUNCTION()
	void SpawnHands();
};
