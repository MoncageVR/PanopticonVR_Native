// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Interface/IGrabInterface.h"
#include "Core/Component/UGrabComp.h"
#include "ALP.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AALP : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()
	
public:
	AALP();

	virtual void Tick(float DeletaTime) override;

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

#pragma region Getter
	const int32 GetSongNum() const;
	const uint32 GetIsHanding() const;
	const uint32 GetIsAttaching() const;
#pragma endregion;

#pragma region Setter
	void SetSongNum(int32 TempSongNum);
	void SetIsAttaching(uint32 TempbIsAttaching);

	void SetMeshCollisionEnabled(uint32 CollisionEnabled);

#pragma endregion


protected:
	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLLP;

private:
	int32 SongNum = 0; // SongNum;
	uint32 bIsHanding = 0; // In Handing? 
	uint32 bIsAttaching = 0; // In TT Attaching?

	FTimerHandle SelfDestroyTimer;

	void DestroyLPSelf();

};
