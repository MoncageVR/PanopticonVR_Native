#include "CoreObj/Manager/GameInstanceSubSystem/VRDialogueManagerSubsystem.h"
#include "Engine/DataTable.h"
#include "CoreCommon/Struct/FDialogueInfoRow.h"

void UVRDialogueManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	DTDialogueTuto1 = LoadObject<UDataTable>(nullptr, TEXT("/Game/VRContent/Prisoner/DialogueSheet/PVR_DialogueData_Tuto1.PVR_DialogueData_Tuto1"));
	DTDialogueTuto2 = LoadObject<UDataTable>(nullptr, TEXT("/Game/VRContent/Prisoner/DialogueSheet/PVR_DialogueData_Tuto2.PVR_DialogueData_Tuto2"));
	DTDialogueTuto3 = LoadObject<UDataTable>(nullptr, TEXT("/Game/VRContent/Prisoner/DialogueSheet/PVR_DialogueData_Tuto3.PVR_DialogueData_Tuto3"));
}

void UVRDialogueManagerSubsystem::StartDialogue()
{
	PlayDialogue(DTDialogueTuto1);
}

void UVRDialogueManagerSubsystem::StartTuto01Dialogue()
{
	OnTutorialEvent.Broadcast(5); // Game Start Lever Disabled!
	PlayDialogue(DTDialogueTuto1);
}

void UVRDialogueManagerSubsystem::StartTuto02Dialogue()
{
	PlayDialogue(DTDialogueTuto2);
}

void UVRDialogueManagerSubsystem::StartTuto03Dialogue()
{
	PlayDialogue(DTDialogueTuto3);
}

void UVRDialogueManagerSubsystem::PlayDialogue(UDataTable* InDT)
{
	if (!InDT) return;

	CurrentDT = InDT;
	RowNames = InDT->GetRowNames();
	CurrentIndex = 0;

	PlayCurrentLine();
}

void UVRDialogueManagerSubsystem::PlayCurrentLine()
{

	if (!CurrentDT || !RowNames.IsValidIndex(CurrentIndex))
	{
		//UE_LOG(LogTemp, Log, TEXT("[Dialogue] All lines finished."));
		FDialogueChanged.Broadcast(FText::FromString(TEXT("")), 100.0f, 100.0f, 100); // "Clear Content" Section After Outputting All Lines
		return;
	}

	FDialogueInfoRow* Row = CurrentDT->FindRow<FDialogueInfoRow>(RowNames[CurrentIndex], TEXT(""));
	if (Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("Num : %d | text_en : %s | Sound : %d | text_time : %f | text_total_en : %f"), Row->num, *Row->text_en.ToString(), (Row->sound - 1), Row->text_time, Row->text_total_en);

		if (Row->text_total_en / 3.0f <= 1.0f)
		{
			DelayTime = 1.0f;
		}
		else
		{
			DelayTime = Row->text_total_en / 3.0f;
		}

		FDialogueChanged.Broadcast(Row->text_en, Row->text_time, Row->text_total_en, Row->sound - 1);
	}
}

void UVRDialogueManagerSubsystem::NotifyLineFinished()
{
	if (UWorld* mWorld = GetWorld())
	{
		mWorld->GetTimerManager().SetTimer(
			NextLineDelayTimer,
			this,
			&UVRDialogueManagerSubsystem::ProceedAfterLine,
			DelayTime,
			false
		);
	}
}

void UVRDialogueManagerSubsystem::ProceedAfterLine()
{
	const int32 TempFinishedIndex = CurrentIndex;
	CurrentIndex++;

	if (TryTriggerTutorialEvent(TempFinishedIndex))
	{
		return;
	}

	if (bIsPaused)
	{
		bPendingNextLine = true;
		return;
	}

	PlayCurrentLine();
}

void UVRDialogueManagerSubsystem::PauseDialogue()
{
	if (bIsPaused) return;
	bIsPaused = true;
	OnDialoguePauseToggle.Broadcast(true);
}

void UVRDialogueManagerSubsystem::ResumeDialogue()
{
	if (!bIsPaused) return;
	bIsPaused = false;
	OnDialoguePauseToggle.Broadcast(false);

	if (bPendingNextLine)
	{
		bPendingNextLine = false;
		PlayCurrentLine();
	}
}

bool UVRDialogueManagerSubsystem::TryTriggerTutorialEvent(int32 FinishedIndex)
{
	bWaitingForEvent = true;
	switch (FinishedIndex)
	{
	case 0:
		//UE_LOG(LogTemp, Warning, TEXT("Tuto1 Num : 1 - Dialogue Print End!"));
		OnTutorialEvent.Broadcast(1); // Change Screen Jack Mode
		return true;
	case 2:
		//UE_LOG(LogTemp, Warning, TEXT("Tuto1 Num : 3 - Dialogue Print End!"));
		OnTutorialEvent.Broadcast(3); // Tape Injection Sequence Play And Change Screen Default Mode
		return true;
	case 3:
		//UE_LOG(LogTemp, Warning, TEXT("Tuto1 Num : 4 - Dialogue Print End!"));
		OnTutorialEvent.Broadcast(1); // Change Screen Jack Mode
		return true;
	case 6:
		//UE_LOG(LogTemp, Warning, TEXT("Tuto1 Num : 7 - Dialogue Print End!"));
		OnTutorialEvent.Broadcast(2); // Change Screen Default Mode
		return true;
	case 7:
		//UE_LOG(LogTemp, Warning, TEXT("Tuto1 Num : 8 - Dialogue Print End!"));
		OnTutorialEvent.Broadcast(4); // Arrow Indicating the Tape Self
		return true;
	case 8:
		OnTutorialEvent.Broadcast(6); // Game Start Lever Enabled!
		return true;
	default:
		return false;
	}
}

void UVRDialogueManagerSubsystem::NotifyTutorialEventFinished()
{
	if (bWaitingForEvent)
	{
		bWaitingForEvent = false;
		PlayCurrentLine();
	}
}