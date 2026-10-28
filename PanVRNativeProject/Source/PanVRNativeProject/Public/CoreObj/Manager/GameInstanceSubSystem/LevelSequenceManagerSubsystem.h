#pragma once

#include "CoreMinimal.h"
#include "CoreObj/Manager/GameInstanceSubSystem/VRGameInstanceSubsystem.h"
#include "LevelSequenceManagerSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnSequenceFinishedSignature);

class ULevelSequence;
class ULevelSequencePlayer;
class ALevelSequenceActor;

UCLASS()
class PANVRNATIVEPROJECT_API ULevelSequenceManagerSubsystem : public UVRGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void PlaySequence(ULevelSequence* Sequence, FOnSequenceFinishedSignature OnFinished);

	void OnlyPlaySequence(ULevelSequence* InSequence);

	void PlayBeamProjSequence(ULevelSequence* Sequence, FOnSequenceFinishedSignature OnFinished);

	void OnlyPlayBeamProjSequence(ULevelSequence* Sequence);

public:
	FOnSequenceFinishedSignature PendingCallback;

protected:

private:
	UFUNCTION()
	void HandleSequenceFinished();

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> CurrentPlayer;

	UPROPERTY()
	TObjectPtr<ALevelSequenceActor> CurrentActor;

	UPROPERTY()
	TObjectPtr<class AABeamProjectors> DynBeamProjector;
	
};
