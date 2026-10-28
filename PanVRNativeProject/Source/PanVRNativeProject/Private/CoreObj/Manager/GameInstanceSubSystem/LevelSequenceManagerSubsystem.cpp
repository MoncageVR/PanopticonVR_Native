#include "CoreObj/Manager/GameInstanceSubSystem/LevelSequenceManagerSubsystem.h"
#include "EquipmentActor/Support_Actors/ABeamProjectors.h"
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

void ULevelSequenceManagerSubsystem::OnlyPlaySequence(ULevelSequence* InSequence)
{
    if (!InSequence) return;

    ALevelSequenceActor* OutActor = nullptr;
    FMovieSceneSequencePlaybackSettings Settings;
    ULevelSequencePlayer* LocalPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
        GetWorld(), InSequence, Settings, OutActor);

    if (LocalPlayer)
    {
        LocalPlayer->Play();
    }
}

void ULevelSequenceManagerSubsystem::PlayBeamProjSequence(ULevelSequence* Sequence, FOnSequenceFinishedSignature OnFinished)
{
    if (!Sequence) return;

    if (CurrentPlayer && CurrentPlayer->IsPlaying()
        && CurrentActor && CurrentActor->GetSequence() == Sequence)
    {
        return;
    }

    PendingCallback = OnFinished;

    if (DynBeamProjector)
    {
        DynBeamProjector->SetActorLocation(FVector(0.f, 0.f, 2180.f));
        DynBeamProjector->SetActorRotation(FQuat(FRotator::ZeroRotator));

        ALevelSequenceActor* SeqActorB = nullptr;
        FMovieSceneSequencePlaybackSettings Settings;
        CurrentPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
            GetWorld(), Sequence, Settings, SeqActorB);

        CurrentActor = SeqActorB;

        if (CurrentPlayer && CurrentActor)
        {
            CurrentPlayer->OnFinished.AddDynamic(this, &ULevelSequenceManagerSubsystem::HandleSequenceFinished);
            SeqActorB->SetBindingByTag(FName("BeamProj"), { DynBeamProjector });
            CurrentPlayer->Play();
        }
    }
}

void ULevelSequenceManagerSubsystem::OnlyPlayBeamProjSequence(ULevelSequence* Sequence)
{
    if (!Sequence) return;

    if (CurrentPlayer && CurrentPlayer->IsPlaying()
        && CurrentActor && CurrentActor->GetSequence() == Sequence)
    {
        return;
    }

    FTransform TempTransform = FTransform(FRotator(0.f), FVector(0.f, 0.f, 2180.f), FVector::OneVector);
    DynBeamProjector = GetWorld()->SpawnActor<AABeamProjectors>(AABeamProjectors::StaticClass(), TempTransform);

    ALevelSequenceActor* SeqActorA = nullptr;
    FMovieSceneSequencePlaybackSettings Settings;
    CurrentPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
        GetWorld(), Sequence, Settings, SeqActorA);

    CurrentActor = SeqActorA;

    if (CurrentPlayer && CurrentActor)
    {
        SeqActorA->SetBindingByTag(FName("BeamProj"), { DynBeamProjector });
        CurrentPlayer->Play();
    }
}

void ULevelSequenceManagerSubsystem::HandleSequenceFinished()
{
    if (IsValid(DynBeamProjector))
    {
        UE_LOG(LogTemp, Warning, TEXT("BeamProjector Related Sequence Play End, And BeamProjector Actor Destroy!"));
        DynBeamProjector->Destroy();
        DynBeamProjector = nullptr;
    }

    if (CurrentPlayer)
        CurrentPlayer->OnFinished.RemoveDynamic(this, &ULevelSequenceManagerSubsystem::HandleSequenceFinished);

    PendingCallback.ExecuteIfBound(); 
    PendingCallback.Unbind();
    CurrentPlayer = nullptr;
}
