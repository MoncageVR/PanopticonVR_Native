// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacterBase.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API AVRCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AVRCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<class USceneComponent> Root;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerMoveUpInLobby();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerMoveDownInLobby();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerMoveUpNDownInStage(bool InDirection);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerMoveDownInStage();
};
