#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MapObjManagerSubsystem.generated.h"

class AAGrating;

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

public:
#pragma region Getter
	FORCEINLINE TArray<FRotator> GetFinalAllGratingSpawnRotations() const { return FinalAllGratingSpawnRotations; }
	FORCEINLINE TArray<FVector> GetFinalAllGratingSpawnPositions() const { return FinalAllGratingSpawnPositions; }
	FORCEINLINE TMap<int32, AAGrating*> GetGratingsMap() const { return GratingsMap; }
#pragma endregion

protected:
	UPROPERTY()
	TMap<int32, AAGrating*> GratingsMap;

protected:
	void InitGratingBaseSpawnValue();
	void InitGratingSpawnTransform();

private:
	class UPrisonerManagerSubsystem* PrisonerManagerSubSystemRef;

	TArray<FRotator> BaseGratingSpawnRotations;
	TArray<FVector> BaseGratingSpawnPositions;

	TArray<FRotator> FinalAllGratingSpawnRotations;
	TArray<FVector> FinalAllGratingSpawnPositions;

};