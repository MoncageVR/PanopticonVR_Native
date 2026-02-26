// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "ACarCrane.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AACarCrane : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	AACarCrane();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComp")
	TObjectPtr<USceneComponent> CarCraneMainRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMeshComp")
	TObjectPtr<class UStaticMeshComponent> UFOMainBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMeshComp")
	TObjectPtr<class UStaticMeshComponent> UFOPortal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMeshComp")
	TObjectPtr<class UStaticMeshComponent> UFOCraneHead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLUFOSubdue;






private:

};
