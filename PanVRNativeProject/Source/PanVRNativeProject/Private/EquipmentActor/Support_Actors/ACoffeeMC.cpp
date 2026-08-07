#include "EquipmentActor/Support_Actors/ACoffeeMC.h"

AACoffeeMC::AACoffeeMC()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_CMCBody(TEXT("/Game/VRContent/Modeling/07_CoffeeMC/SM_CoffeeMCBody.SM_CoffeeMCBody"));
	if (ModelingFinder_CMCBody.Succeeded())
		ActorBaseMesh->SetStaticMesh(ModelingFinder_CMCBody.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_CMCHandle(TEXT("/Game/VRContent/Modeling/07_CoffeeMC/SM_CoffeeMCHandle.SM_CoffeeMCHandle"));
	CoffeeMCHandle = CreateDefaultSubobject<UStaticMeshComponent>("SM_Handle");
	if (CoffeeMCHandle)
	{
		CoffeeMCHandle->SetupAttachment(ActorBaseMesh);
		if (ModelingFinder_CMCHandle.Succeeded())
		{
			CoffeeMCHandle->SetStaticMesh(ModelingFinder_CMCHandle.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MatFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MatFinder_Main.Object);
		CoffeeMCHandle->SetMaterial(0, MatFinder_Main.Object);
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
