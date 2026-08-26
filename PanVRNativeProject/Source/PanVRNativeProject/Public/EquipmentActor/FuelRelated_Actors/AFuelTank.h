#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "AFuelTank.generated.h"

class AAFuelRod;

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAFuelTank : public AVRGrabActorBase , public IIGrabInterface
{
	GENERATED_BODY()
	
public:
	AAFuelTank();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTimes) override;

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

	// IIEquipmentInitInterface In Natural Virtual Function Overriding
	// For Use Delegate System 
	virtual void EquipmentRegistrable(AActor* InActor) override;

#pragma region Getter
	FORCEINLINE TObjectPtr<class AAFuelRod> GetCurrEquipFuelRod() const { return EquipFuelRod; }
#pragma endregion

#pragma region Components
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> FTMainRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> FTMeshRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> FTAttachRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> FTMainBody;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> FTCLRod;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> FTCLGrab;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USplineComponent> FTMoveRoute;
#pragma endregion

#pragma region CollisionFunctions
protected:
	// FuelTank Movement For Overlap Check Functions
	UFUNCTION()
	void GrabColOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void GrabColOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	// FuelRod Attaching Check Functions
	UFUNCTION()
	void FuelRodColOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void FuelRodColOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
#pragma endregion

protected:
	UFUNCTION()
	void MoveOperateFuelTank();

private:
	FTimerHandle FuelTankMoveTimer;

	FTimerHandle FuelTankSoundPlayTimer;

	// Capsule Collision In Handing Check Flag Variable
	bool bIsHanding = false;

	UPROPERTY()
	TObjectPtr<class UMotionControllerComponent> TempMCRef = nullptr;

	uint32 bIsOpen = 0;
	uint32 bIsAttachObjExist = 0;

	UPROPERTY()
	TObjectPtr<class AAFuelRod> EquipFuelRod;

	UPROPERTY()
	TObjectPtr<USoundBase> SFXFuelTankMove;
	UPROPERTY()
	TObjectPtr<USoundBase> SFXFuelRodIn;
	UPROPERTY()
	TObjectPtr<USoundBase> SFXFuelRodOut;
};
