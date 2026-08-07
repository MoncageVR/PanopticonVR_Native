

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Component/UGrabComp.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "APickle.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAPickle : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()

public:
	AAPickle();

	virtual void BeginPlay() override;

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

protected:

private:
	UPROPERTY()
	TObjectPtr<class USkeletalMeshComponent> SKM_CucumberBody;
	UPROPERTY()
	TObjectPtr<class UCapsuleComponent> CL_CucumberBody;

	FTimerHandle DestroyTimer;

private:
	UFUNCTION()
	void DestroySelfActor();
};
