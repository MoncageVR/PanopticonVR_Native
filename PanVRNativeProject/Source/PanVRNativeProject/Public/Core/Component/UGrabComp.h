// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UGrabComp.generated.h"

UENUM(BlueprintType)
enum class EGrabType : uint8
{
	Free UMETA(DisplayName = "Free"),
	ObjToHand UMETA(DisplayName = "ObjToHand"),
	HandToObj UMETA(DisplayName = "HandToObj"),
	SpawnObj UMETA(DisplayName = "SpawnObj"),
	AutomaticObj UMETA(DisplayName = "AutomaticObj")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PANVRNATIVEPROJECT_API UUGrabComp : public USceneComponent
{
	GENERATED_BODY()

public:	
	UUGrabComp(); // Constructor

	// Try Grab Function
	UFUNCTION()
	void GCTryGrab(class UMotionControllerComponent* InMC, class USkeletalMeshComponent* InSMComp, class AVRHand* InGrabbingHand);

	// Try Release Function
	UFUNCTION()
	bool GCTryRelease();

	// Setter
	// Parent Component(= Primitive Component) Set Physics Simulating
	UFUNCTION(BlueprintCallable)
	void SetPrimitiveCompPhysics(bool InSimulate);

protected:
	virtual void BeginPlay() override;

	// If this Component Attached Primitive Component is Physics Simulating
	// PrimitiveComponent Physics Simulating True Setting Function
	UFUNCTION(BlueprintCallable)
	void SetShouldSimulateOnDrop();

	// Attach To Component Function Call By Free , ObjToHand for Function
	UFUNCTION(BlueprintCallable)
	void AttachGCParentToMotionController(class UMotionControllerComponent* InMC, USceneComponent* InTargetObj);

	// Attach To Component Function Call By HandToObj Using Function
	UFUNCTION(BlueprintCallable)
	void AttachHandToGCParent(class USkeletalMeshComponent* InSMComp, USceneComponent* InAttachParent);

	// Attach To Component Function Call By ObjToHand Using Function
	UFUNCTION(BlueprintCallable)
	void AttachGCParentToHand(class UMotionControllerComponent* InMCRef);

public:	
	// Check Vairable Item is Hold 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Values")
	bool bIsHeld;

	// Physics Simulating Flag Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Values")
	bool bSimulateOnDrop;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Values")
	FRotator PrimaryGrabRelativeRotation;

	// ENum Class Variable : GrabType
	/*
	Free
	ObjToHand
	HandToObj
	SpawnObj
	AutomaticObj
	*/
	UPROPERTY(EditAnywhere, Category = "Values|GrabType")
	EGrabType EEGrabType;

	// Current Grabbed Motion Controller Component Reference Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Values|MC")
	TObjectPtr<class UMotionControllerComponent> MCRef;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AVRHand> MHand;

	// Current Grabbed SkeletalMesh Component Reference Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Values|Mesh")
	TObjectPtr<class USkeletalMeshComponent> GrabbedBySkeletalMesh;

	// Current Grabbed GrabInterface Class Reference Variable
	UPROPERTY()
	TScriptInterface<class IIGrabInterface> CurrentIGrabInterface;

#pragma region Getter
	// MotionController Reference Vairable Getter Function
	UFUNCTION()
	const class UMotionControllerComponent* GetMCRef();

	// bIsHeld Variable Getter Function
	UFUNCTION(BlueprintCallable)
	EControllerHand GetHeldByHand();
#pragma endregion

private:
};
