// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "AClipboard.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API AClipboard : public AVRGrabActorBase
{
	GENERATED_BODY()

public:
	AClipboard();

protected:

#pragma region Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clipboard|Mesh")
	TObjectPtr<class UStaticMeshComponent> SM_CB_Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clipboard|Mesh")
	TObjectPtr<class UStaticMeshComponent> SM_CB_Paper;
#pragma endregion

#pragma region Hierarchy
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clipboard")
	TObjectPtr<class USceneComponent> GC_CB;
#pragma endregion

#pragma region Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clipboard|Collision")
	TObjectPtr<class UBoxComponent> CL_Box_Paper;
#pragma endregion
};
