#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "ABeamProjectors.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API AABeamProjectors : public AVRActorBase
{
	GENERATED_BODY()

public:
	AABeamProjectors();

	virtual void BeginPlay() override;

protected:

#pragma region Components
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MainRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BP_Parts2;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BP_Parts3;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BP_PlaneParts1;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BP_PlaneParts2;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BP_PlaneParts3;

#pragma endregion


private:

	
};
