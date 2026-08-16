#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "ASodaBullet.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AASodaBullet : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()

public:
	AASodaBullet(); // Constructor
	virtual void BeginPlay() override;

	void SetActorPhysics(bool PhysicsFlag);

	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

	void CurveMove(FVector InDir);

#pragma region Setter
	void SetSBIsAttaching(uint32 TempFlag);
#pragma endregion

#pragma region Getter
	uint32 GetSBIsAttaching() const;
#pragma endregion

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLBox;

	UFUNCTION()
	void DestroySelf();

private:
	// SodaBullet Not Using Check Timer
	FTimerHandle DestroySelfTimer;

	uint32 bIsSBAttaching = 0;

private:
	
};
