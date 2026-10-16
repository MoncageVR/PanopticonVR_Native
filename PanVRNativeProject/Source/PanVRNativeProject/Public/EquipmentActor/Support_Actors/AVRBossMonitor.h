#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AVRBossMonitor.generated.h"

UCLASS()
class AAVRBossMonitor : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	AAVRBossMonitor();

	virtual void BeginPlay() override;

protected:


private:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> SM_Glass;

};
