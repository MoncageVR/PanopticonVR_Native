// Fill out your copyright notice in the Description page of Project Settings.

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

protected:

	/*UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent>*/

private:

	
};
