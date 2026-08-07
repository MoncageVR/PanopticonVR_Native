

#include "EquipmentActor/Support_Actors/AClipboard.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

AClipboard::AClipboard()
{
	PrimaryActorTick.bCanEverTick = false;

	// Root
	this->SetRootComponent(ActorBaseMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ClipboardBody(TEXT("/Game/VRContent/Modeling/06_Clipboard/SM_Clipboard.SM_Clipboard"));
	if (ModelingFinder_ClipboardBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_ClipboardBody.Object);
	}

	GC->SetupAttachment(ActorBaseMesh);

	// Hierarchy node attached to body
	GC_CB = CreateDefaultSubobject<USceneComponent>(TEXT("GC_CB"));
	GC_CB->SetupAttachment(ActorBaseMesh);

	// Paper mesh under GC_CB
	SM_CB_Paper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_CB_Paper"));
	SM_CB_Paper->SetupAttachment(GC_CB);

	// Collision on paper surface
	CL_Box_Paper = CreateDefaultSubobject<UBoxComponent>(TEXT("CL_Box_Paper"));
	CL_Box_Paper->SetupAttachment(SM_CB_Paper);
	CL_Box_Paper->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CL_Box_Paper->SetGenerateOverlapEvents(true);
	CL_Box_Paper->SetHiddenInGame(false);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MatFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MatFinder_Main.Object);
	}

	TArray<UPrimitiveComponent*> AllComps;
	GetComponents<UPrimitiveComponent>(AllComps);
	for (UPrimitiveComponent* AllComp : AllComps)
	{
		if (!AllComp) continue;

		if (AllComp->CanEverAffectNavigation())
			AllComp->SetCanEverAffectNavigation(false);
		else
			continue;
	}
}
