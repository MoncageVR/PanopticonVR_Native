#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "AClipboard.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API AClipboard : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()

public:
	AClipboard();

	virtual void BeginPlay() override;

	// GrabInterface Function override Part
#pragma region IIGrabInterFace
	// IGrabInterface Function override
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;
#pragma endregion

protected:

#pragma region Mesh
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clipboard|Mesh")
	TObjectPtr<class UStaticMeshComponent> SM_CB_Body;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clipboard|Mesh")
	TObjectPtr<class UStaticMeshComponent> SM_CB_Paper;
#pragma endregion

#pragma region Hierarchy
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clipboard")
	TObjectPtr<class USceneComponent> GC_CB;*/
#pragma endregion

#pragma region Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clipboard|Collision")
	TObjectPtr<class UBoxComponent> CL_Box_Paper;
#pragma endregion

private:
	uint32 CurrPageNum = 0;
	FVector AtFirstActorPos;
	FRotator AtFirstActorRot;

	bool bIsHanding = false;

	UPROPERTY()
	TObjectPtr<class ACVRPawn> mVRPawnPlayer;

	UPROPERTY()
	TObjectPtr<class AVRGameMode> CurrVRGM;

	UPROPERTY()
	TObjectPtr<class AVRLobbyGameMode> CurrVRLobbyGM;

	UPROPERTY()
	TObjectPtr<class UCapsuleComponent> ColCapsule;

	UPROPERTY()
	TObjectPtr<class ATowerBuilding> TowerBuildingRef;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstance>> MI_StageSpritesArrs;

private:
	UFUNCTION()
	void CBBoxOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
