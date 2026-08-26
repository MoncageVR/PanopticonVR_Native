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
		return;
	}

	FDialogueInfoRow* Row = CurrentDT->FindRow<FDialogueInfoRow>(RowNames[CurrentIndex], TEXT(""));
	if (Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("Num : %d | text_en : %s | Sound : %d | text_time : %f | text_total_en : %f"), Row->num, *Row->text_en.ToString(), (Row->sound-1), Row->text_time, Row->text_total_en);
		FDialogueChanged.Broadcast(Row->text_en, Row->text_time, Row->text_total_en, Row->sound - 1);
	}
}


void UVRDialogueManagerSubsystem::NotifyLineFinished()
{
	CurrentIndex++;
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
