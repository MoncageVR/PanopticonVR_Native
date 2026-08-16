#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AScopeCamera.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAScopeCamera : public AVRActorBase
{
	GENERATED_BODY()

public:
	AAScopeCamera();

	virtual void BeginPlay() override;
	// IIEquipmentInitInterface In Natural Virtual Function Overriding , For Use Delegate System 
	virtual void EquipmentRegistrable(AActor* InActor) override;

protected:

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SC_MainRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> SceneCaptureCameraBody;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_HMD;

private:

};
