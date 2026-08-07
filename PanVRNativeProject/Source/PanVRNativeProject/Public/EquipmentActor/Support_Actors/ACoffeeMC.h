#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "ACoffeeMC.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API AACoffeeMC : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	AACoffeeMC();

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> CoffeeMCHandle;

private:
};