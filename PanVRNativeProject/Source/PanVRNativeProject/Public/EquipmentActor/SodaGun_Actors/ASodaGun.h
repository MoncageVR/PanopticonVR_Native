#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "ASodaGun.generated.h"


UCLASS()
class PANVRNATIVEPROJECT_API AASodaGun : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()
	
public:
	AASodaGun();

	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;

	virtual void OnDropped() override;

protected:
	virtual void BeginPlay() override;

	// SodaCan Reload And Load Check Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CL_SodaCanLoadBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Scene")
	TObjectPtr<class USceneComponent> MuzzleStandardLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Scene")
	TObjectPtr<class USceneComponent> EjaculationStandardLocation;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void LoadCheckColOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void LoadCheckColOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

private:
	UPROPERTY()
	TObjectPtr<AActor> DefaultParentActor;
	UPROPERTY()
	TObjectPtr<class AASodaBullet> EquipSodaBullet;
	UPROPERTY()
	TObjectPtr<USoundBase> SFXReloadCan;
	UPROPERTY()
	TObjectPtr<USoundBase> SFXShoot;

	FTransform DefaultTransform;
	uint32 AttachingSodaBullet = 0;
	uint32 bIsLoading = 0; // SodaBullet Load Check Flag

#pragma region InputRelated
	UPROPERTY()
	TObjectPtr<APlayerController> mPlayerCon; // VRPawn Controller = FirstPlayerController
	
	UPROPERTY()
	TObjectPtr<ULocalPlayer> mLocalPlayer; // VRPawn = FirstPlayer
	UPROPERTY()
	TObjectPtr<class UEnhancedInputLocalPlayerSubsystem> InputSubSystem;
	UPROPERTY()
	TObjectPtr<class UEnhancedInputComponent> EIComp;

	// IMC(InputMappingContext) Variables
	UPROPERTY()
	TObjectPtr<class UInputMappingContext> IMCRight;
	UPROPERTY()
	TObjectPtr<class UInputMappingContext> IMCLeft;

	// IA(InputAction) Variables
	UPROPERTY()
	TObjectPtr<class UInputAction> IARightShoot;
	UPROPERTY()
	TObjectPtr<class UInputAction> IALeftShoot;
#pragma endregion
	UPROPERTY()
	TSubclassOf<AActor> SodaGunBulletSmoke;

private:
	void OnShootFunc();
	void ShootAfterEjaculationFunc();

};
