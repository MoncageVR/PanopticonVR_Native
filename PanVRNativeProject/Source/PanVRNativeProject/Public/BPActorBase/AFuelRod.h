// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Component/UGrabComp.h"
#include "Core/Interface/IGrabInterface.h"
#include "Core/Interface/IDestroyInterface.h"
#include "AFuelRod.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAFuelRod : public AVRGrabActorBase , public IIGrabInterface , public IIDestroyInterface
{
	GENERATED_BODY()
	
public:
	AAFuelRod();
#pragma region IFunctions
	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;
	virtual void DestroySelf() override;
#pragma endregion

	virtual void Tick(float DeltaTime) override;

#pragma region Setter
	void SetFRIsAttaching(uint32 TempAttaching);
#pragma endregion

#pragma region Getter
	uint32 GetFRIsAttaching() const ;
#pragma endregion

#pragma region TimelineFunc
	void GaugeOperationStart();
	void GaugeOperationStop();

	UFUNCTION()
	void GaugeOperatingTimeline(float Value);

#pragma endregion

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> FRCLStick;

private:
	uint32 bIsAttaching = 0;
	class UTimelineComponent* GaugeTimelineComp;
	UCurveFloat* GaugeMoveCurve;
	USoundBase* SFXFuelRodLowGauge;
	bool bWasLowGauge = false;
};
