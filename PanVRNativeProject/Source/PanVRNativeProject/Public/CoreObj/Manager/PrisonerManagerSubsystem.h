// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PrisonerManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UPrisonerManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void CreateAllPrisoner(); // Spawns all prisoners and initializes their Blackboard data.

public:
#pragma region Getter
	FORCEINLINE TArray<FRotator> GetFinalAllSpawnRoations() const { return FinalAllSpawnRotations; }
	FORCEINLINE TArray<FVector> GetFinalAllSpawnPositions() const { return FinalAllSpawnPositions; }
	FORCEINLINE int32 GetPrisonerTotalNum() const { return InGamePrisonerTotalNum; }
	FORCEINLINE float GetPrisonerSpeed() const { return PrisonerRunSpeed; }
	FORCEINLINE TMap<uint8, uint8> GetOppositeUniqueNumMaps() const { return OppositeUniqueNumMaps; }
	FORCEINLINE TArray<FRotator> GetBaseSpawnRotations() const { return BaseSpawnRotations; }
	FORCEINLINE TArray<FVector> GetBaseSpawnPositions() const { return BaseSpawnPositions; }
#pragma endregion
#pragma region Setter

#pragma endregion

protected:
	void InitAllValues();
	void InitPrisonerBaseSpawnValue();
	void InitPrisonerSpawnTransform();
	void InitPrisonerReSpawnTransform();
	void InitOppositePrisonerNumber();

private:
	int32 InGamePrisonerTotalNum = 24; // Total Number of Prisoner
	float PrisonerRunSpeed = 189.0f; // Prisoner Running Speed

	float Radius = 2000.0f; // 
	float ZPos = 321.f; // 
	float HeightBetweenFloor = 950.0f; // Distance between Floors

	FVector EscapeTargetPosition = FVector(-2000.0f, 0.0f, 287.0f);

	TArray<FRotator> BaseSpawnRotations; // 123FSpawnRot
	TArray<FVector> BaseSpawnPositions; // 123FSpawnVec

	TArray<FRotator> FinalAllSpawnRotations;
	TArray<FVector> FinalAllSpawnPositions;

	TArray<FVector> FinalAllReSpawnPositions;

	TMap<uint8, uint8> OppositeUniqueNumMaps;

	//TMap<uint8, uint8> Debug_State_;
};
