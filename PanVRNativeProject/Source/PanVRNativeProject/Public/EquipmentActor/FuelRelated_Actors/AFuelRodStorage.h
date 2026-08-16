#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Component/UGrabComp.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "AFuelRodStorage.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAFuelRodStorage : public AVRGrabActorBase , public IIGrabInterface
{
	GENERATED_BODY()
	

public:
	AAFuelRodStorage();

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> FRCLStorage;

	UPROPERTY()
	TObjectPtr<class AAFuelRod> SpawnNewFR;

private:
	
};
