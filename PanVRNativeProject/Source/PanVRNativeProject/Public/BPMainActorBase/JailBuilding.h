// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JailBuilding.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API AJailBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	AJailBuilding();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	// Scene Component Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComponent")
	TObjectPtr<USceneComponent> JailMainRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComponent")
	TObjectPtr<USceneComponent> FirstFloorRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComponent")
	TObjectPtr<USceneComponent> SecondFloorRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComponent")
	TObjectPtr<USceneComponent> ThirdFloorRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComponent")
	TObjectPtr<USceneComponent> MoveableBuildingsRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComponent")
	TObjectPtr<USceneComponent> StaticBuildingsRoot;

	// Static Mesh Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class UStaticMeshComponent> JailMainBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class UStaticMeshComponent> JailExitDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class UStaticMeshComponent> JailHatch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class UStaticMeshComponent> JailRoof;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class UStaticMeshComponent> Jail1FWeaponDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class UStaticMeshComponent> Jail2FWeaponDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<class UStaticMeshComponent> Jail3FWeaponDoor;

	// Collision Component Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLSubdueHatch;
};
