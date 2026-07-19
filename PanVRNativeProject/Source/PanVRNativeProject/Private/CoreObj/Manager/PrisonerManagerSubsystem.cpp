// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreObj/Manager/PrisonerManagerSubsystem.h"
#include "Core/Character/PrisonerCharacter.h"
#include "Core/Character/PrisonerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DataTable.h"
#include "Core/Enum/EPrisonerStates.h"

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
	PrisonerRunSpeed = 189.0f; // Debug Value : 500.0f , Default Value = 189.0f;
	Radius = 2000.0f;
	ZPos = 321.0f;
	HeightBetweenFloor = 950.0f;
	EscapeTargetPosition = FVector(-2000.0f, 0.0f, 261.0f);
	PhenomenonOccurProbability = 0.02f;
	RandomChoice = 0;

	PrisonerPossibleNumbers.Empty();
	PrisonerPossibleNumbers.Reserve(InGamePrisonerTotalNum);

	DTGroupA = LoadObject<UDataTable>(nullptr, TEXT("/Game/VRContent/Prisoner/LogicSheet/BaseSheets/PanOpticon_Prisoner_Logic1_GroupA.PanOpticon_Prisoner_Logic1_GroupA"));
	DTGroupB = LoadObject<UDataTable>(nullptr, TEXT("/Game/VRContent/Prisoner/LogicSheet/BaseSheets/PanOpticon_Prisoner_Logic1_GroupB.PanOpticon_Prisoner_Logic1_GroupB"));
	DTGroupC = LoadObject<UDataTable>(nullptr, TEXT("/Game/VRContent/Prisoner/LogicSheet/BaseSheets/PanOpticon_Prisoner_Logic1_GroupC.PanOpticon_Prisoner_Logic1_GroupC"));
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
		TempPrisonerCon->SetMyLogicDT(DTGroupArrs[i]);

		// Debug!
		if (i == 0)
		{
			// Interact(3) - DoorPicking(8)
			TempPrisonerCon->GetBBComp()->SetValueAsEnum(TEXT("CurrUpperState"), 4);
			TempPrisonerCon->GetBBComp()->SetValueAsEnum(TEXT("CurrLowerState"), 14);

			uint8 TempBPUpperValue = TempPrisonerCon->GetBBComp()->GetValueAsEnum(TEXT("CurrUpperState"));
			EPrisonerUpperState TempCPPUpperValue = EPrisonerUpperState::DANGEROUS;

			uint8 TempBPLowerValue = TempPrisonerCon->GetBBComp()->GetValueAsEnum(TEXT("CurrLowerState"));
			EPrisonerLowerState TempCPPLowerValue = EPrisonerLowerState::RADIOACTIVITY;

			if (TempBPUpperValue == static_cast<uint8>(TempCPPUpperValue))
			{
				UE_LOG(LogTemp, Log, TEXT("%d Prisoner Upper State Enum Match!"),i);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("%d Prisoner Upper State Enum NOT Match!"),i);
			}

			if (TempBPLowerValue == static_cast<uint8>(TempCPPLowerValue))
			{
				UE_LOG(LogTemp, Log, TEXT("%d Prisoner Lower State Enum Match!"), i);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("%d Prisoner Lower State Enum NOT Match!"), i);
			}


			// Move(2) - Run(5)
			//TempPrisonerCon->GetBBComp()->SetValueAsEnum(TEXT("CurrUpperState"), 2);
			//TempPrisonerCon->GetBBComp()->SetValueAsEnum(TEXT("CurrLowerState"), 5);

			//// Dangerous(4) - Escape(13)
			//TempPrisonerCon->GetBBComp()->SetValueAsEnum(TEXT("CurrUpperState"), 4);
			//TempPrisonerCon->GetBBComp()->SetValueAsEnum(TEXT("CurrLowerState"), 13);
		}
	}
}

void UPrisonerManagerSubsystem::Create_Paranormal_Phenomenon()
{
	float TempWeight = (float)(PhenomenonOccurProbability * InGamePrisonerTotalNum);
	bool bIsPhenomenonResult = true; //bool bIsPhenomenonResult = FMath::FRand() < TempWeight;

	if (bIsPhenomenonResult)
	{
		//UE_LOG(LogTemp, Log, TEXT("Phenonmenon Occur"));

		// Random Number Create And Choice
		RandomChoice = FMath::RandRange(0, InGamePrisonerTotalNum - 1);

		//UE_LOG(LogTemp, Log, TEXT("Random Choice : %d"), RandomChoice);

		if (PrisonerPossibleNumbers[RandomChoice] != -1)
		{
			PrisonerPossibleNumbers[RandomChoice] = -1;
			//UE_LOG(LogTemp, Log, TEXT("Prisoner Logic Play Part!"));
			AllPrisonerControllerArrs[RandomChoice]->HandlePrisonerLogic(RandomChoice);
		}
	}
	else
	{
		return;
	}
}
