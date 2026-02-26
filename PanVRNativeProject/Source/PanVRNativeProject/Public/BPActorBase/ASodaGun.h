// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Interface/IGrabInterface.h"
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
	AActor* DefaultParentActor;
	FTransform DefaultTransform;
	class AASodaBullet* EquipSodaBullet;
	USoundBase* SFXReloadCan;
	USoundBase* SFXShoot;

	uint32 AttachingSodaBullet = 0;
	uint32 bIsLoading = 0; // SodaBullet Load Check Flag

#pragma region InputRelated
	APlayerController* mPlayerCon; // VRPawn Controller = FirstPlayerController
	ULocalPlayer* mLocalPlayer; // VRPawn = FirstPlayer
	class UEnhancedInputLocalPlayerSubsystem* InputSubSystem;
	class UEnhancedInputComponent* EIComp;

	// IMC(InputMappingContext) Variables
	class UInputMappingContext* IMCRight;
	class UInputMappingContext* IMCLeft;

	// IA(InputAction) Variables
	class UInputAction* IARightShoot;
	class UInputAction* IALeftShoot;
#pragma endregion

	TSubclassOf<AActor> SodaGunBulletSmoke;

private:
	void OnShootFunc();
	void ShootAfterEjaculationFunc();

};
