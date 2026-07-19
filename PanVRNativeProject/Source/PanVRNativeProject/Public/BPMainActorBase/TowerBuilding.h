// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "TowerBuilding.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API ATowerBuilding : public AVRActorBase
{
	GENERATED_BODY()
	
public:	
	ATowerBuilding();

	virtual void BeginPlay() override;
	virtual void EquipmentRegistrable(AActor* InActor) override;
	virtual void Tick(float DeltaTime) override;

public:
#pragma region Getter

	int32 GetTowerCurrFloorNum() const;

#pragma endregion

#pragma region Setter

	void SetTowerCurrFloorNum(int32 InCurrFloor);

#pragma endregion

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SceneComp")
	TObjectPtr<USceneComponent> MainRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> TowerMainBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> MainDesk;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> SubDesk;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|StaticMesh")
	TObjectPtr<UStaticMeshComponent> Barricade;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLSubdueForToilet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLSubdueForKeyPad;

	UPROPERTY()
	TObjectPtr<class UAudioComponent> TBAudioPlayer;

protected:
	UFUNCTION()
	void HandleTowerReceiveByEB(FName InTag, int32 InFloor);

	

	UFUNCTION()
	void ActuallyTowerMoveCompleted();

private:
	int32 ActuallyCurrFloorNum = 3;
	TArray<float> TargetTowerHeights;
	FLatentActionInfo TowerLatentInfo;

	USoundBase* TowerMoveSFXCue;

private:
	void ActuallyMoveTower(float TargetTowerHeight);

};
