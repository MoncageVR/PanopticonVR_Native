// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "ACoffeeMC.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AACoffeeMC : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	AACoffeeMC();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> CoffeeMCHandle;

private:

};
