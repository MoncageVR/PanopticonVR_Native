#include "CoreObj/Manager/GameInstanceSubSystem/MapObjManagerSubsystem.h"
#include "CoreObj/Manager/GameInstanceSubSystem/PrisonerManagerSubsystem.h"
#include "CoreObj/Manager/VRGameInstance.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "MainActor/AGrating.h"
#include "MainActor/JailBuilding.h"
#include "EquipmentActor/Spawned_Actors/APickle.h"
#include "EquipmentActor/Spawned_Actors/AGolfEffect.h"

void UMapObjManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency<UPrisonerManagerSubsystem>();

	InitGratingBaseSpawnValue();
	InitGratingSpawnTransform();

	PrisonerManagerSubSystemRef = GetWorld()->GetGameInstance()->GetSubsystem<UPrisonerManagerSubsystem>();
	check(PrisonerManagerSubSystemRef);
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

	/*if (UWorld* MyWorld = GetWorld())
	{
		TempGrating = MyWorld->SpawnActor<AAGrating>(AAGrating::StaticClass(), FinalAllGratingSpawnPositions[0], FinalAllGratingSpawnRotations[0]);
	}*/

	for (int32 i = 0; i < PrisonerManagerSubSystemRef->GetPrisonerTotalNum(); i++)
	{
		TempGrating = GetWorld()->SpawnActor<AAGrating>(AAGrating::StaticClass(), this->FinalAllGratingSpawnPositions[i], this->FinalAllGratingSpawnRotations[i]);
		TempGrating->SetIndividualNum(i);
		GratingsMap.Add(i, TempGrating);
	}
}

void UMapObjManagerSubsystem::ControlExitDoorFunction()
{
	UVREquipmentWorldSubsystem* TempVREquipMgrWolrdSubSyPtr = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	check(TempVREquipMgrWolrdSubSyPtr);

	AJailBuilding* TempJailBuilding = nullptr;
	for (TScriptInterface<IIEquipmentInitInterface> Equip : TempVREquipMgrWolrdSubSyPtr->GetEquipmentArr())
	{
		IIEquipmentInitInterface* IEquipPtr = Equip.GetInterface();
		TempJailBuilding = Cast<AJailBuilding>(IEquipPtr);
		if (TempJailBuilding)
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

void UMapObjManagerSubsystem::HandleEraseGolfEffect()
{
	if (!GolfEffectArrs.IsEmpty()) // If GolfEffect exists in the world,
	{
		if (GolfEffectArrs.Num() > 0)
		{
			AAGolfEffect* FirstGolfEffect = GolfEffectArrs[0];
			UE_LOG(LogTemp, Log, TEXT("Erasing GolfEffect: %s"), *FirstGolfEffect->GetName());
			if (IsValid(FirstGolfEffect))
			{
				FirstGolfEffect->Destroy();
				GolfEffectArrs.RemoveAt(0);
			}
		}
	}
	else // If GolfEffects does not exist in the world,
	{
		return;
	}

	//if()
}

void UMapObjManagerSubsystem::HandleSpawnPickleObj()
{
	GetWorld()->SpawnActor<AAPickle>(AAPickle::StaticClass(), FTransform(FRotator::ZeroRotator, FVector(82.0f, 0.f, 2250.0f), FVector::OneVector));

	return;
}
