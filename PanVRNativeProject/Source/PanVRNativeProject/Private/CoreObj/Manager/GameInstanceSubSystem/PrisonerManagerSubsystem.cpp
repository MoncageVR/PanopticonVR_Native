#include "CoreObj/Manager/GameInstanceSubSystem/PrisonerManagerSubsystem.h"
#include "CoreCommon/PrisonerRelated/PrisonerCharacter.h"
#include "CoreCommon/PrisonerRelated/PrisonerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DataTable.h"
#include "CoreCommon/Enum/EPrisonerStates.h"
#include "CoreCommon/Struct/FPrisonerInfoRow.h"

void UPrisonerManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	InitAllValues();
	InitPrisonerBaseSpawnValue();
	InitPrisonerSpawnTransform();
	InitPrisonerReSpawnTransform();
	InitOppositePrisonerNumber();
}

void UPrisonerManagerSubsystem::InitAllValues()
{
	InGamePrisonerTotalNum = 24;
	PrisonerRunSpeed = 500.0f; // Debug Value : 500.0f , Default Value = 189.0f;
	Radius = 2000.0f;
	ZPos = 321.0f;
	HeightBetweenFloor = 950.0f;
	EscapeTargetPosition = FVector(-2000.0f, 0.0f, 261.0f);
	PhenomenonOccurProbability = 0.02f;
	RandomChoice = 0;

	PrisonerPossibleNumbers.Empty();
	PrisonerPossibleNumbers.Reserve(InGamePrisonerTotalNum);

	DTGroupA = LoadObject<UDataTable>(nullptr, TEXT("/Game/VRContent/Prisoner/LogicSheet/PanOpticon_Prisoner_A_Logic1_Group.PanOpticon_Prisoner_A_Logic1_Group"));
	DTGroupB = LoadObject<UDataTable>(nullptr, TEXT("/Game/VRContent/Prisoner/LogicSheet/PanOpticon_Prisoner_B_Logic1_Group.PanOpticon_Prisoner_B_Logic1_Group"));
	DTGroupC = LoadObject<UDataTable>(nullptr, TEXT("/Game/VRContent/Prisoner/LogicSheet/PanOpticon_Prisoner_C_Logic1_Group.PanOpticon_Prisoner_C_Logic1_Group"));
}

void UPrisonerManagerSubsystem::InitPrisonerBaseSpawnValue()
{
	// 123F Spawn Rotations And Positions Base Value Initialize
	BaseSpawnRotations.Empty();
	BaseSpawnRotations.Reserve(8);

	BaseSpawnPositions.Empty();
	BaseSpawnPositions.Reserve(8);

	for (int32 i = 0; i < 8; ++i)
	{
		float CalculatedYaw = 0.0f;
		if (i < 4)
			CalculatedYaw = (i + 1) * 36.0f;
		else
			CalculatedYaw = (i + 2) * 36.0f;
		
		if (CalculatedYaw > 144.0f)
			CalculatedYaw -= 360.0f;

		BaseSpawnRotations.Add(FRotator(0.0f, CalculatedYaw, 0.0f));
	}
}

void UPrisonerManagerSubsystem::InitPrisonerSpawnTransform()
{
	// Allocate space in TArray's
	FinalAllSpawnPositions.Empty();
	FinalAllSpawnPositions.Reserve(InGamePrisonerTotalNum);

	FinalAllSpawnRotations.Empty();
	FinalAllSpawnRotations.Reserve(InGamePrisonerTotalNum);

	for (int32 i = 0; i < 3; i++)
	{
		for (FRotator var : BaseSpawnRotations)
		{
			float TempRadianAngle = FMath::DegreesToRadians(var.Yaw);

			BaseSpawnPositions.Add
			(
				FVector(
					FMath::Cos(TempRadianAngle) * Radius, 
					FMath::Sin(TempRadianAngle) * Radius, 
					ZPos
				)
			);
			FinalAllSpawnPositions.Add(FVector(FMath::Cos(TempRadianAngle) * Radius, FMath::Sin(TempRadianAngle) * Radius, ZPos));

			FinalAllSpawnRotations.Add(FRotator(0.f, var.Yaw + 180.0f, 0.f));
		}

		ZPos += HeightBetweenFloor;
	}
}

void UPrisonerManagerSubsystem::InitPrisonerReSpawnTransform()
{
	float ReSpawnRadius = 3100.f;
	float ReSpawnZPos = 1600.f;

	for (int32 i = 0; i < 3; i++)
	{
		for (FRotator var : BaseSpawnRotations)
		{
			float TempRadianAngle = FMath::DegreesToRadians(var.Yaw);

			FinalAllReSpawnPositions.Add(
				FVector(
					FMath::Cos(TempRadianAngle) * ReSpawnRadius , 
					FMath::Sin(TempRadianAngle) * ReSpawnRadius , 
					ReSpawnZPos
				)
			);
		}

		ReSpawnZPos += HeightBetweenFloor;
	}
}

void UPrisonerManagerSubsystem::InitOppositePrisonerNumber()
{
	OppositeUniqueNumMaps.Empty();

	const int32 NumPerFloor = 8;
	const int32 TotalFloor = 3;

	for (int32 Floor = 0; Floor < TotalFloor; Floor++)
	{
		int32 BaseIndex = Floor * NumPerFloor;

		for (int32 i = 0; i < NumPerFloor; i++)
		{
			uint8 Key = BaseIndex + i;
			uint8 Value = BaseIndex + ((i + 4) % NumPerFloor);

			OppositeUniqueNumMaps.Add(Key, Value);
		}
	}
}

void UPrisonerManagerSubsystem::ClassifyUniqueNumByRotOfEachZone(int32 InIndex, float InYaw)
{
	if ((FMath::IsNearlyEqual(InYaw, 144.0f, 2.0f)) || (FMath::IsNearlyEqual(InYaw, -144.0f, 10.0f)))
	{
		AlphaZonePrisonerUniqueNumArrs.Add(InIndex);
	}
	else if ((FMath::IsNearlyEqual(InYaw, -108.0f, 2.0f)) || (FMath::IsNearlyEqual(InYaw, -72.0f, 2.0f)) || (FMath::IsNearlyEqual(InYaw, -36.0f, 2.0f)))
	{
		BetaZonePrisonerUniqueNumArrs.Add(InIndex);
	}
	else if ((FMath::IsNearlyEqual(InYaw, 108.0f, 2.0f)) || (FMath::IsNearlyEqual(InYaw, 72.0f, 2.0f)) || (FMath::IsNearlyEqual(InYaw, 36.0f, 2.0f)))
	{
		GammaZonePrisonerUniqueNumArrs.Add(InIndex);
	}
	return;
}

void UPrisonerManagerSubsystem::CreateAllPrisoner()
{
	APrisonerCharacter* TempPrisonerCha = nullptr;
	APrisonerController* TempPrisonerCon = nullptr;

	for (int32 i = 0; i < 8; i++)
	{
		DTGroupArrs.Add(DTGroupA);
		DTGroupArrs.Add(DTGroupB);
		DTGroupArrs.Add(DTGroupC);
	}
	for (int32 i = 0; i < DTGroupArrs.Num(); i++)
	{
		int32 RanIndex = FMath::RandRange(0, DTGroupArrs.Num() - 1);
		DTGroupArrs.Swap(i, RanIndex);
	}

	for (int32 i = 0; i < InGamePrisonerTotalNum; i++)
	{
		PrisonerPossibleNumbers.Add(i);

		TempPrisonerCha = GetWorld()->SpawnActor<APrisonerCharacter>(APrisonerCharacter::StaticClass(), this->FinalAllSpawnPositions[i], this->FinalAllSpawnRotations[i]);

		TempPrisonerCon = Cast<APrisonerController>(TempPrisonerCha->GetController());
		AllPrisonerControllerArrs.Add(TempPrisonerCon);

		TempPrisonerCon->GetBBComp()->SetValueAsInt(TEXT("UniqueNum"), i); // Set Individual Unique Number
		TempPrisonerCon->GetBBComp()->SetValueAsVector(TEXT("SpawnVec"), this->FinalAllSpawnPositions[i]); // Set Individual Spawn Vector
		TempPrisonerCon->GetBBComp()->SetValueAsFloat(TEXT("RunningSpeed"), PrisonerRunSpeed); // Set Common Running Speed
		TempPrisonerCon->GetBBComp()->SetValueAsVector(TEXT("EscapeTargetVec"), EscapeTargetPosition); // Set Common Escape Target Position
		TempPrisonerCon->GetBBComp()->SetValueAsInt(TEXT("OppositeUniqueNum"), OppositeUniqueNumMaps[i]); // Set Individual Opposite Unique Number
		TempPrisonerCon->GetBBComp()->SetValueAsVector(TEXT("RandomMoveTargetVec"), FVector(0, 0, 0)); // Set Individual RandomMove Target Position
		TempPrisonerCon->GetBBComp()->SetValueAsVector(TEXT("TeleportTargetVec"), FVector(1300.0f, 0.0f, 321.504486f)); // Set Individual Teleport Target Position
		TempPrisonerCon->GetBBComp()->SetValueAsVector(TEXT("CeilingEscapeTargetVec"), FVector(2100.f, 0.f, 2100.f));
		TempPrisonerCon->GetBBComp()->SetValueAsVector(TEXT("TopEscapeTargetVec"), FVector(0.f, 0.f, 3700.f));
		TempPrisonerCon->GetBBComp()->SetValueAsBool(TEXT("bIsPatrolBlocked"), false);

		TempPrisonerCon->SetMyLogicDT(DTGroupArrs[i]);

		this->ClassifyUniqueNumByRotOfEachZone(i, TempPrisonerCha->GetActorRotation().Yaw);

		if (TempPrisonerCon->GetMyLogicDT() == DTGroupA)
		{
			TempPrisonerCha->SetPrisonerAppearanceByDT(true, true, false);
		}
		else if (TempPrisonerCon->GetMyLogicDT() == DTGroupB)
		{
			TempPrisonerCha->SetPrisonerAppearanceByDT(false, false, true);
		}
		else if (TempPrisonerCon->GetMyLogicDT() == DTGroupC)
		{
			TempPrisonerCha->SetPrisonerAppearanceByDT(false, false, false);
			TempPrisonerCha->HandleSetPrisonerNewSkin();
		}

		// Debug!
		if (i == 0)
		{
			TempPrisonerCon->SetMyLogicDT(DTGroupA);

			TempPrisonerCon->InitializeStatesFromLogicDT();

			//// Interact(3) - DoorPicking(8)
			//TempPrisonerCon->GetBBComp()->SetValueAsEnum(TEXT("CurrUpperState"), 4);
			//TempPrisonerCon->GetBBComp()->SetValueAsEnum(TEXT("CurrLowerState"), 14);

			uint8 TempBPUpperValue = TempPrisonerCon->GetBBComp()->GetValueAsEnum(TEXT("CurrUpperState"));
			EPrisonerUpperStateType TempCPPUpperValue = EPrisonerUpperStateType::Dangerous;

			uint8 TempBPLowerValue = TempPrisonerCon->GetBBComp()->GetValueAsEnum(TEXT("CurrLowerState"));
			EPrisonerLowerStateType TempCPPLowerValue = EPrisonerLowerStateType::Radioactivity;
		}
		// Debug
	}

	AlphaZonePrisonerUniqueNumArrs.Sort();
	BetaZonePrisonerUniqueNumArrs.Sort();
	GammaZonePrisonerUniqueNumArrs.Sort();
}

void UPrisonerManagerSubsystem::Create_Paranormal_Phenomenon()
{
	UE_LOG(LogTemp, Log, TEXT("10 Seconds Passing, Paranormal Phenomenon"));

	float TempWeight = (float)(PhenomenonOccurProbability * InGamePrisonerTotalNum);
	bool bIsPhenomenonResult = true; //bool bIsPhenomenonResult = FMath::FRand() < TempWeight;

	if (bIsPhenomenonResult)
	{
		// Random Number Create And Choice
		RandomChoice = FMath::RandRange(0, InGamePrisonerTotalNum - 1);
		UE_LOG(LogTemp, Log, TEXT("Random Choice : %d"), RandomChoice);

		// Debug 0 Number Prisoner Fixed Call
		AllPrisonerControllerArrs[0]->HandlePlayPrisonerLogic(0);
		//AllPrisonerControllerArrs[16]->HandlePlayPrisonerLogic(16);

		//if (PrisonerPossibleNumbers[RandomChoice] != -1)
		//{
		//	PrisonerPossibleNumbers[RandomChoice] = -1;
		//	//UE_LOG(LogTemp, Log, TEXT("Prisoner Logic Play Part!"));
		//	AllPrisonerControllerArrs[RandomChoice]->HandlePlayPrisonerLogic(RandomChoice);
		//}
	}
	else
	{
		return;
	}
}
