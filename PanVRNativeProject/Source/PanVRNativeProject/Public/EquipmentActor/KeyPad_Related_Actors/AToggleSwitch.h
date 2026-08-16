#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AToggleSwitch.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAToggleSwitch : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	AAToggleSwitch();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTimes) override;
	
	// True : BackMove(Switch On) , False : FrontMove(Switch Off)
	void HandleToggleSwitch(uint8 InMoveFlag);

	virtual void EquipmentRegistrable(AActor* InActor) override;

protected:
	UFUNCTION()
	void OnSwitchOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_Switch;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> CL_Switch;

	UPROPERTY()
	TObjectPtr<USoundBase> SFX_SwitchOn;
	UPROPERTY()
	TObjectPtr<USoundBase> SFX_SwitchOff;

	uint8 bIsSwitching;
private:
	
};
