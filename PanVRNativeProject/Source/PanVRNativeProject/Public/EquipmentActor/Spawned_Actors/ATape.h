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

	void HandleDontGrabPhysics(uint8 bIsDontGrabFlag);

#pragma region Getter
	uint8 GetTapeTypeNum() const { return TapeType; }
	uint32 GetTapeNum() const { return TapeNum; }
#pragma endregion

#pragma region Setter
	void SetTapeType(uint8 InTypeNum) { TapeType = InTypeNum; }
	void SetTapeNum(uint32 InNum) { TapeNum = InNum; }
	void SetTapeInfo(uint8 InTypeNum, uint32 InNum, int32 InMatNum)
	{
		TapeType = InTypeNum;
		TapeNum = InNum;
		ActorBaseMesh->SetMaterial(0, TapeMats[InMatNum]);
	}
	// Possible On Grab Tape
	void SetTapeGrabOn();
	/*
	SetTapeInfo(0,0,0) => Tutorial Tape 2
	SetTapeInfo(0,1,1) => Tutorial Tape 3
	SetTapeInfo(1,0,2) => Main Level Tape 1
	*/
#pragma endregion

protected:
	UPROPERTY(EditInstanceOnly)
	uint8 TapeType = 0; // 0 = Tutorial , 1 = Main Level

	UPROPERTY(EditInstanceOnly)
	uint32 TapeNum = 0;
	/*
	TapeType : 0 , TapeNum : 0 , MatNum : 0 - Tuto Level 2
	TapeType : 0 , TapeNum : 1 , MatNum : 1 - Tuto Level 3
	TapeType : 1 , TapeNum : 0 , MatNum : 2 - Main Level 1
	*/

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstance>> TapeMats;

private:

};
