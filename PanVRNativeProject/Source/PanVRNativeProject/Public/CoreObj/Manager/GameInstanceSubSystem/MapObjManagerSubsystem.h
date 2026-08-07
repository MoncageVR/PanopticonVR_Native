#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MapObjManagerSubsystem.generated.h"

class AAGrating;
class AAGolfEffect;

/**
 *
 */
UCLASS()
class PANVRNATIVEPROJECT_API UMapObjManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void CreateAllGratings();
	void ControlExitDoorFunction();

public:
	void HandleFlyingTheGrating(int32 InGratingNum);

	UFUNCTION()
	void HandleEraseGolfEffect();

	void HandleSpawnPickleObj();

public:
#pragma region Getter
	FORCEINLINE TArray<FRotator> GetFinalAllGratingSpawnRotations() const { return FinalAllGratingSpawnRotations; }
	FORCEINLINE TArray<FVector> GetFinalAllGratingSpawnPositions() const { return FinalAllGratingSpawnPositions; }
	FORCEINLINE TMap<int32, AAGrating*> GetGratingsMap() const { return GratingsMap; }
	FORCEINLINE TArray<TObjectPtr<AAGolfEffect>> GetGolfEffectArrs() const { return GolfEffectArrs; }
#pragma endregion

#pragma region GolfEffectArrayRelated
public:
	void AddGolfEffect(AAGolfEffect* InEffect) { GolfEffectArrs.Add(InEffect); }
	void RemoveGolfEffect(AAGolfEffect* InEffect) { GolfEffectArrs.Remove(InEffect); }
#pragma endregion

protected:
	UPROPERTY()
	TMap<int32, AAGrating*> GratingsMap;

	UPROPERTY()
	TArray<TObjectPtr<AAGolfEffect>> GolfEffectArrs;

protected:
	void InitGratingBaseSpawnValue();
	void InitGratingSpawnTransform();

private:
	UPROPERTY()
	class UPrisonerManagerSubsystem* PrisonerManagerSubSystemRef;

	TArray<FRotator> BaseGratingSpawnRotations;
	TArray<FVector> BaseGratingSpawnPositions;

	TArray<FRotator> FinalAllGratingSpawnRotations;
	TArray<FVector> FinalAllGratingSpawnPositions;

};