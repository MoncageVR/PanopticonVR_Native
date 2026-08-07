#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "Kismet/GameplayStatics.h"
#include "AEmergencyButton.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAEmergencyButton : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	AAEmergencyButton();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> EMButton;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CLButton;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USplineComponent> SCButtonMoveRail;

	UFUNCTION()
	void EMButtonColOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	UPROPERTY()
	TObjectPtr<USoundBase> ButtonSuccessSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> ButtonFailSFX;

	uint32 bIsHitting = 0;
	FTimerHandle ReturnTimer;

	UPROPERTY(VisibleAnywhere)
	TArray<uint32> StopButtonFinalOutputIntArrays;

private:
	// CoolDown EmergencyButton 
	void OperateAfterReturn();

	// KeyPad -> EmergencyButton
	void HandleThisReceiveByKeyPad(bool bIsFlag, uint32 InputIndex);

	// EmergencyButton In OutputInArray Clear And KeyPad OutputArray Clear
	void ClearStopButtonOutputArray();
};
