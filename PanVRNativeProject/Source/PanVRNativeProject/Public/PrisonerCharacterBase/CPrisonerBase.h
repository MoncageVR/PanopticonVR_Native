// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPrisonerBase.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API ACPrisonerBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACPrisonerBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> SMMoustache;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> SMBeard;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> SMHair;

};
