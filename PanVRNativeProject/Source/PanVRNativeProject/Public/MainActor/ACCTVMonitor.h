#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "ACCTVMonitor.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AACCTVMonitor : public AVRActorBase
{
	GENERATED_BODY()

public:
	AACCTVMonitor();

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SC_MainRoot;

	UPROPERTY(VisibleAnywhere);
	TObjectPtr<UStaticMeshComponent> CCTVArm;

	UPROPERTY(VisibleAnywhere);
	TObjectPtr<UStaticMeshComponent> CCTVGlass;
	
};
