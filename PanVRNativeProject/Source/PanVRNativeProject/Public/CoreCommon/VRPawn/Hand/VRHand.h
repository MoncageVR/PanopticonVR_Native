
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRHand.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API AVRHand : public AActor
{
	GENERATED_BODY()
	
public:	
	AVRHand();

	UFUNCTION(BlueprintCallable)
	void ReleaseObject();

protected:
	virtual void OnConstruction(const FTransform& InTransform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GrabObject();

	UFUNCTION()
	void AfterReleaseResetHandMesh();

// Components Variables Part
#pragma region Components
	// The actual SkeletalMesh component of the hand
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Mesh")
	TObjectPtr<class USkeletalMeshComponent> HandMesh;

	// Collision Box Located In the Middle of the Hand
	// Role : Check Collisions with the GC(GrabComponent)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLBox;

	// Collision Capsule Located In the Index Finger of the Hand
	// Role : Instead of Grab Object, Pressing Object For Interaction 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UCapsuleComponent> CLCapsuleIndex;

	// Role : Instead of Pressing Object, Grab Object For Interaction
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class USphereComponent> CLGrabSphere;

	// VR Interaction With Motion Controller For Player Interaction
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|MotionController")
	TObjectPtr<class UMotionControllerComponent> MC;
#pragma endregion

// HandData Variables Part
#pragma region HandData
	
	// Which Hand is It Define
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|Hand|HandData")
	EControllerHand HandType;

	// Check Flag Hand Animation Mirroring
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|Hand|HandData")
	bool bMirrorAnimation;

	// Save Variable Back Original Transform For Left Hand
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Hand|HandData")
	FTransform AtFirstLeftHandTransform;

	// Save Variable Back Original Transform For Right Hand
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Hand|HandData")
	FTransform AtFirstRightHandTransform;
#pragma endregion


private:
	TObjectPtr<class UUGrabComp> GrabbleActorGC;

private:

#pragma region Getter
public:
	UCapsuleComponent* GetCLCapsuleIndex() const;
#pragma endregion
};
