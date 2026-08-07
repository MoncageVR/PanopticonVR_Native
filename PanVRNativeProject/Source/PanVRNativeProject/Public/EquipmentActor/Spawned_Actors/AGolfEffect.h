

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AGolfEffect.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAGolfEffect : public AVRActorBase
{
	GENERATED_BODY()

public:
	AAGolfEffect();
	virtual void BeginPlay() override;

	bool MakeGolfEggEffect();

protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> SC_MainRoot;

private:
	UPROPERTY()
	TObjectPtr<class UMapObjManagerSubsystem> MapObjManagerSubSystemRef;

private:
};
