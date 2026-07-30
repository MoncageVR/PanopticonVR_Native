// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AGolfClub.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAGolfClub : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	AAGolfClub();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> ClubMainRoot;


protected:	

private:
};
