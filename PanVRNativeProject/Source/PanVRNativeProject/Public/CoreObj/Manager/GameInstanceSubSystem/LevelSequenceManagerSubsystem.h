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

	
};
