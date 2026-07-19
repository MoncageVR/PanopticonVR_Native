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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void HandleGaugeOperation(bool bIsOperFlag);

#pragma region IFunctions
	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;
	virtual void DestroySelf() override;
#pragma endregion

#pragma region Getter
	FORCEINLINE uint32 GetFRIsAttaching() const { return bIsAttaching; }
	FORCEINLINE bool GetWasLowGaugeFlag() const { return bWasLowGauge; }
#pragma endregion
#pragma region Setter
	FORCEINLINE void SetFRIsAttaching(uint32 TempAttaching) { bIsAttaching = TempAttaching; }
#pragma endregion

#pragma region TimelineFunc
	/*void GaugeOperationStart();
	void GaugeOperationStop();*/

protected:
	UFUNCTION()
	void GaugeOperatingTimeline(float Value);
#pragma endregion

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> FRCLStick;

private:
	uint32 bIsAttaching = 0;
	class UTimelineComponent* GaugeTimelineComp;
	UCurveFloat* GaugeMoveCurve;
	USoundBase* SFXFuelRodLowGauge;
	bool bWasLowGauge = false;

private:

};
