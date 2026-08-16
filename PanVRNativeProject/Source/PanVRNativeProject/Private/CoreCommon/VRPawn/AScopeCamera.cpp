#include "CoreCommon/VRPawn/AScopeCamera.h"
#include "CoreCommon/VRPawn/CVRPawn.h"
#include "CoreCommon/Interface/IEquipmentInitInterface.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

AAScopeCamera::AAScopeCamera()
{
	PrimaryActorTick.bCanEverTick = false;

	SC_MainRoot = CreateDefaultSubobject<USceneComponent>("MainRoot_SceneComp");
	if (SC_MainRoot)
	{
		this->SetRootComponent(SC_MainRoot);
	}

	SceneCaptureCameraBody = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	if (SceneCaptureCameraBody)
	{
		SceneCaptureCameraBody->SetupAttachment(SC_MainRoot);
	}

	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetupAttachment(SceneCaptureCameraBody);
		ActorBaseMesh->SetRelativeLocation(FVector(25.0f, 0.f, 0.f));
		ActorBaseMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

		static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Hole(TEXT("/Game/VRContent/Modeling/13_Keypad/Periscope_SM_UIHole.Periscope_SM_UIHole"));
		static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Scope(TEXT("/Game/VRContent/Material/SRS_STAGE_Periscope.SRS_STAGE_Periscope"));
		if (ModelingFinder_Hole.Succeeded() && MatFinder_Scope.Succeeded())
		{
			ActorBaseMesh->SetStaticMesh(ModelingFinder_Hole.Object);
			ActorBaseMesh->SetMaterial(0, MatFinder_Scope.Object);
		}
	}

	SM_HMD = CreateDefaultSubobject<UStaticMeshComponent>("HMDSMComp");
	if (SM_HMD)
	{
		SM_HMD->SetupAttachment(SceneCaptureCameraBody);
		SM_HMD->SetCollisionProfileName(FName("OverlapAll"));
		SM_HMD->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SM_HMD->SetHiddenInGame(true);
		SM_HMD->SetRelativeLocation(FVector(11.0f, 0.0f, 0.0f));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_HMD(TEXT("/Engine/VREditor/Devices/Generic/GenericHMD.GenericHMD"));
		if (ModelingFinder_HMD.Succeeded())
			SM_HMD->SetStaticMesh(ModelingFinder_HMD.Object);
	}
}

void AAScopeCamera::BeginPlay()
{
	Super::BeginPlay();
	EquipmentRegistrable(this);
}

void AAScopeCamera::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}