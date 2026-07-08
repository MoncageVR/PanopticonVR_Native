// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Component/UGrabComp.h"
#include "Core/Interface/IGrabInterface.h"
#include "Core/Interface/IDestroyInterface.h"
#include "APaper.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAPaper : public AVRGrabActorBase , public IIGrabInterface , public IIDestroyInterface
{
	GENERATED_BODY()
	
public:
	AAPaper();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTimes) override;

	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

	virtual void DestroySelf() override;

	void SetCanGrabPhysics(uint32 InFlag);

	void StampOn();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CLA4;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UTextRenderComponent> TR_CharPrinter;

private:
	uint32 bIsHanding = 0;
	uint32 bIsPrinterAttaching = 0;
	uint32 bIsStampAttaching = 0;
	uint32 bIsFaxAttaching = 0;
	uint32 bIsPrinting = 0;
	uint32 bIsStamping = 0;

	USoundBase* SFXPaperGrab;

private:

public:
#pragma region Setter
	void SetIsPrinterAttaching(uint32 InFlag);
	void SetIsStampAttaching(uint32 InFlag);
	void SetIsFaxAttaching(uint32 InFlag);
	void SetIsPrinting(uint32 InFlag);
	void SetIsStamping(uint32 InFlag);
#pragma endregion

#pragma region Getter
	uint32 GetIsPrinterAttaching();
	uint32 GetIsStampAttaching();
	uint32 GetIsFaxAttaching();
	uint32 GetIsPrinting();
	uint32 GetIsStamping();
#pragma endregion
};
