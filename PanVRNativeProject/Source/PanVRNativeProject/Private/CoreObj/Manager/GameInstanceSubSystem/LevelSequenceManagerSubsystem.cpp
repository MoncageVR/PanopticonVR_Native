#include "CoreObj/Manager/GameInstanceSubSystem/LevelSequenceManagerSubsystem.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

void ULevelSequenceManagerSubsystem::PlaySequence(ULevelSequence* Sequence, FOnSequenceFinishedSignature OnFinished)
{
    if (!Sequence) return;

    if (CurrentPlayer && CurrentPlayer->IsPlaying()
        && CurrentActor && CurrentActor->GetSequence() == Sequence)
    {
        return;
    }

    PendingCallback = OnFinished;

    ALevelSequenceActor* OutActor = nullptr;
    FMovieSceneSequencePlaybackSettings Settings;
    CurrentPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
        GetWorld(), Sequence, Settings, OutActor);
    CurrentActor = OutActor;

    if (CurrentPlayer)
    {
        CurrentPlayer->OnFinished.AddDynamic(this, &ULevelSequenceManagerSubsystem::HandleSequenceFinished);
        CurrentPlayer->Play();
    }
}

void ULevelSequenceManagerSubsystem::HandleSequenceFinished()
{
    if (CurrentPlayer)
        CurrentPlayer->OnFinished.RemoveDynamic(this, &ULevelSequenceManagerSubsystem::HandleSequenceFinished);

    PendingCallback.ExecuteIfBound(); 
    PendingCallback.Unbind();
    CurrentPlayer = nullptr;
}
