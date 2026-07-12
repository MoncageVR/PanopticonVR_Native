#include "CoreObj/Manager/MapObjManagerSubsystem.h"
#include "CoreObj/Manager/PrisonerManagerSubsystem.h"
#include "BPMainActorBase/AGrating.h"
#include "BPMainActorBase/JailBuilding.h"
#include "CoreObj/VRGameInstance.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"

void UMapObjManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency<UPrisonerManagerSubsystem>();

	InitGratingBaseSpawnValue();
	InitGratingSpawnTransform();
}

void UMapObjManagerSubsystem::InitGratingBaseSpawnValue()
{
	// 123F Spawn Rotations And Positions Base Value Initialize
	BaseGratingSpawnRotations.Empty();
	BaseGratingSpawnRotations.Reserve(8);

	BaseGratingSpawnPositions.Empty();
	BaseGratingSpawnPositions.Reserve(8);

	for (int32 i = 0; i < 8; ++i)
	{
		float CalculatedYaw = 0.0f;
		if (i < 4)
			CalculatedYaw = (i + 1) * 36.0f;
		else
			CalculatedYaw = (i + 2) * 36.0f;

		if (CalculatedYaw > 144.0f)
			CalculatedYaw -= 360.0f;

		BaseGratingSpawnRotations.Add(FRotator(0.0f, CalculatedYaw, 0.0f));
	}
}

void UMapObjManagerSubsystem::InitGratingSpawnTransform()
{
	// Allocate space in TArray's
	FinalAllGratingSpawnPositions.Empty();
	FinalAllGratingSpawnPositions.Reserve(24);

	FinalAllGratingSpawnRotations.Empty();
	FinalAllGratingSpawnRotations.Reserve(24);

	float TempRadius = 1550.0f;
	float TempZPos = 549.0f;
	float TempHeightBetweenFloor = 930.0f;

	for (int32 i = 0; i < 3; i++)
	{
		for (FRotator var : BaseGratingSpawnRotations)
		{
			float TempRadianAngle = FMath::DegreesToRadians(var.Yaw);

			FinalAllGratingSpawnPositions.Add
			(
				FVector(
					FMath::Cos(TempRadianAngle) * TempRadius,
					FMath::Sin(TempRadianAngle) * TempRadius,
					TempZPos
				)
			);
			FinalAllGratingSpawnRotations.Add(FRotator(0.f, var.Yaw, 0.f));
		}

		TempZPos += TempHeightBetweenFloor;
	}
}

void UMapObjManagerSubsystem::CreateAllGratings()
{
	AAGrating* TempGrating = nullptr;

	for (int32 i = 0; i < 23; i++)
	{
		TempGrating = GetWorld()->SpawnActor<AAGrating>(AAGrating::StaticClass(), this->FinalAllGratingSpawnPositions[i], this->FinalAllGratingSpawnRotations[i]);
		TempGrating->SetIndividualNum(i);
		GratingsMap.Add(i, TempGrating);
	}
}

void UMapObjManagerSubsystem::ControlExitDoorFunction()
{
	UVRGameInstance* TempVRGameInst = Cast<UVRGameInstance>(GetWorld()->GetGameInstance());
	UVREquipmentWorldSubsystem* TempVREquipInst = nullptr;
	if (!ensure(TempVRGameInst)) return;
	else TempVREquipInst = TempVRGameInst->GetVREquipmentManager();

	if (!ensure(TempVREquipInst)) return;
	AJailBuilding* TempJailBuilding = nullptr;

	for (auto Var : TempVREquipInst->GetEquipmentArr())
	{
		if (IsValid(TempJailBuilding = Cast<AJailBuilding>(Var)))
			break;
		else
			continue;
	}

	if (TempJailBuilding)
		TempJailBuilding->HandleExitDoor();
}

void UMapObjManagerSubsystem::HandleFlyingTheGrating(int32 InGratingNum)
{
	AAGrating* TempGrating = GratingsMap.FindRef(InGratingNum);
	TempGrating->GratingFly(InGratingNum);
}
