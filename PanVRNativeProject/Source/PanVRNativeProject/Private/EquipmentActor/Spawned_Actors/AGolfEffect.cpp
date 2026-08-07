#include "EquipmentActor/Spawned_Actors/AGolfEffect.h"
#include "MainActor/TowerBuilding.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreObj/Manager/GameInstanceSubSystem/MapObjManagerSubsystem.h"

AAGolfEffect::AAGolfEffect()
{
	PrimaryActorTick.bCanEverTick = false;

	SC_MainRoot = CreateDefaultSubobject<USceneComponent>("MainRootSceneComp");
	if (SC_MainRoot)
	{
		this->SetRootComponent(SC_MainRoot);
		ActorBaseMesh->SetupAttachment(SC_MainRoot);
	}

	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.0f));
		ActorBaseMesh->SetRelativeRotation(FRotator(90.f, 180.f, 0.f));
		ActorBaseMesh->SetCanEverAffectNavigation(false);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Plane(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (SMFinder_Plane.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(SMFinder_Plane.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_GolfEffect(TEXT("/Game/VRContent/Prisoner/GolfRelate/Material/GolfPaintingMat_Inst.GolfPaintingMat_Inst"));
	if (MatFinder_GolfEffect.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MatFinder_GolfEffect.Object);
	}
}

void AAGolfEffect::BeginPlay()
{
	Super::BeginPlay();

	MapObjManagerSubSystemRef = GetWorld()->GetGameInstance()->GetSubsystem<UMapObjManagerSubsystem>();
	check(MapObjManagerSubSystemRef);
}

bool AAGolfEffect::MakeGolfEggEffect()
{
	UVREquipmentWorldSubsystem* TempVREquipmentWorldSubSystemRef = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	ATowerBuilding* TempTowerObj = nullptr;

	check(TempVREquipmentWorldSubSystemRef);
	for (TScriptInterface<IIEquipmentInitInterface> Equip : TempVREquipmentWorldSubSystemRef->GetEquipmentArr())
	{
		IIEquipmentInitInterface* IEquipPtr = Equip.GetInterface();
		TempTowerObj = Cast<ATowerBuilding>(IEquipPtr);
		if (TempTowerObj)
			break;
		else
			continue;
	}

	this->AttachToActor(TempTowerObj, FAttachmentTransformRules::KeepWorldTransform);

	UMaterialInstanceDynamic* TempDynamicMatInst = nullptr;
	FLinearColor RandomColor = FLinearColor(
		FMath::FRandRange(0.0f, 1.0f),  // R
		FMath::FRandRange(0.0f, 1.0f),  // G
		FMath::FRandRange(0.0f, 1.0f),  // B
		1.0f                            // A
	);

	TempDynamicMatInst = this->ActorBaseMesh->CreateDynamicMaterialInstance(0, this->ActorBaseMesh->GetMaterial(0));
	TempDynamicMatInst->SetScalarParameterValue(FName(TEXT("Frame")), FMath::RandRange(0, 3));
	TempDynamicMatInst->SetVectorParameterValue(FName(TEXT("Color")), RandomColor);

	MapObjManagerSubSystemRef->AddGolfEffect(this);

	return true;
}
