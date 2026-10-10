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

#pragma region Getter
	uint32 const GetTapeNum() { return TapeNum; }
#pragma endregion

#pragma region Setter
	void SetTapeNum(uint32 InNum) { TapeNum = InNum; }
#pragma endregion

protected:

private:
	/*UPROPERTY()
	TObjectPtr<class UBoxComponent> CL_TapeBody;*/

	uint32 TapeNum = 1;
};
