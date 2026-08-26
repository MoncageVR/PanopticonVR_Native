#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "CoreCommon/Component/UGrabComp.h"
#include "Kismet/GameplayStatics.h"
#include "VRGrabActorBase.generated.h"

class HVRSoundPlayer;

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AVRGrabActorBase : public AVRActorBase
{
	GENERATED_BODY()

public:
	AVRGrabActorBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UUGrabComp> GC;

	//HVRSoundPlayer* mSoundPlayer;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	TObjectPtr<USoundBase> SFX_HeavyGrab;

	UPROPERTY()
	TObjectPtr<USoundBase> SFX_LightGrab;

private:

};