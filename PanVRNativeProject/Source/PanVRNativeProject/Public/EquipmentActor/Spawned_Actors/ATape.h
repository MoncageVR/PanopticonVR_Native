#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "ATape.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AATape : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()
	
public:
	AATape();
	virtual void BeginPlay() override;

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

	void HandleDontGrabPhysics(uint8 bIsGrabFlag);

protected:

private:
	/*UPROPERTY()
	TObjectPtr<class UBoxComponent> CL_TapeBody;*/

};
