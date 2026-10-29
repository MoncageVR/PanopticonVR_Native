#include "EquipmentActor/Support_Actors/AClipboard.h"
#include "CoreObj/GameMode/VRGameMode.h"
#include "CoreObj/GameMode/VRLobbyGameMode.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreCommon/VRPawn/CVRPawn.h"
#include "CoreCommon/VRPawn/Hand/VRHand.h"
#include "MainActor/TowerBuilding.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

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
	if (GC)
	{
		GC->SetRelativeLocation(FVector(-12.7f, 0.f, -0.7f));
		GC->EEGrabType = EGrabType::ObjToHand;
	}
	SM_CB_Paper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_CB_Paper"));
	if (SM_CB_Paper)
	{
		SM_CB_Paper->SetupAttachment(ActorBaseMesh);
		SM_CB_Paper->SetRelativeLocation(FVector(0.f, 1.4f, 9.1f));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_CB_Paper(TEXT("/Game/VRContent/Modeling/06_Clipboard/Clipboard_Paper.Clipboard_Paper"));
		if (SMFinder_CB_Paper.Succeeded())
		{
			SM_CB_Paper->SetStaticMesh(SMFinder_CB_Paper.Object);
		}
	}

	// Collision on paper surface
	CL_Box_Paper = CreateDefaultSubobject<UBoxComponent>(TEXT("CL_Box_Paper"));
	if (CL_Box_Paper)
	{
		CL_Box_Paper->SetupAttachment(SM_CB_Paper);
		CL_Box_Paper->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CL_Box_Paper->SetGenerateOverlapEvents(true);
		CL_Box_Paper->SetHiddenInGame(true);
		CL_Box_Paper->SetRelativeLocation(FVector(0.f, 0.2f, -9.8f));
		CL_Box_Paper->SetBoxExtent(FVector(7.1f, 0.3f, 10.0f));
		CL_Box_Paper->OnComponentBeginOverlap.AddDynamic(this, &AClipboard::CBBoxOverlapBegin);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MatFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MatFinder_Main.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Paper01(TEXT("/Game/VRContent/Material/MI_Stage_CB_Inst_Paper1.MI_Stage_CB_Inst_Paper1"));
	if (MatFinder_Paper01.Succeeded())
	{
		SM_CB_Paper->SetMaterial(0, MatFinder_Paper01.Object);
	}

	MI_StageSpritesArrs.Empty();
	MI_StageSpritesArrs.Reserve(5);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Paper02(TEXT("/Game/VRContent/Material/MI_Stage_CB_Inst_Paper2.MI_Stage_CB_Inst_Paper2"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Paper03(TEXT("/Game/VRContent/Material/MI_Stage_CB_Inst_Paper3.MI_Stage_CB_Inst_Paper3"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Paper04(TEXT("/Game/VRContent/Material/MI_Stage_CB_Inst_Paper4.MI_Stage_CB_Inst_Paper4"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Paper05(TEXT("/Game/VRContent/Material/MI_Stage_CB_Inst_Paper5.MI_Stage_CB_Inst_Paper5"));

	MI_StageSpritesArrs.Add(MatFinder_Paper01.Object);
	MI_StageSpritesArrs.Add(MatFinder_Paper02.Object);
	MI_StageSpritesArrs.Add(MatFinder_Paper03.Object);
	MI_StageSpritesArrs.Add(MatFinder_Paper04.Object);
	MI_StageSpritesArrs.Add(MatFinder_Paper05.Object);

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

void AClipboard::BeginPlay()
{
	Super::BeginPlay();

	mVRPawnPlayer = nullptr;
	CurrVRLobbyGM = nullptr;
	CurrVRGM = nullptr;

	AtFirstActorPos = ActorBaseMesh->GetRelativeLocation();
	AtFirstActorRot = ActorBaseMesh->GetRelativeRotation();
	CurrPageNum = 0;

	AGameModeBase* CurrGM = UGameplayStatics::GetGameMode(GetWorld());
	check(CurrGM);

	if (Cast<AVRLobbyGameMode>(CurrGM))
	{
		CurrVRLobbyGM = Cast<AVRLobbyGameMode>(CurrGM);
		mVRPawnPlayer = Cast<ACVRPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}
	else if (Cast<AVRGameMode>(CurrGM))
	{
		CurrVRGM = Cast<AVRGameMode>(CurrGM);
		mVRPawnPlayer = Cast<ACVRPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

		for (TScriptInterface<IIEquipmentInitInterface> Equip : EquipmentWorldSubSystem->GetEquipmentArr())
		{
			IIEquipmentInitInterface* IEquipPtr = Equip.GetInterface();
			TowerBuildingRef = Cast<ATowerBuilding>(IEquipPtr);
			if (TowerBuildingRef)
				break;
			else
				continue;
		}
	}
}

void AClipboard::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, AVRHand* InGrabbingHand)
{
	if (GC->MCRef == &InMCRef)
	{
		ColCapsule = InGrabbingHand->GetCLCapsuleIndex();

		if (InMCRef.ComponentHasTag(FName("RightMC")))
		{
			InGrabbingHand->GetCLCapsuleIndex()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ActorBaseMesh->SetRelativeLocationAndRotation(FVector(0.f, -7.0f, 8.0f), FRotator(120.f, 90.f, -90.f));
			bIsHanding = true;
		}
		else
		{
			InGrabbingHand->GetCLCapsuleIndex()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ActorBaseMesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -16.5f), FRotator(-90.f, 90.f, -90.f));
			bIsHanding = true;
		}
	}
}

void AClipboard::OnDropped()
{
	if (CurrVRGM)
	{
		if (ActorBaseMesh->AttachToComponent(TowerBuildingRef->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform))
		{
			ActorBaseMesh->SetRelativeLocationAndRotation(AtFirstActorPos, AtFirstActorRot);
			bIsHanding = false;
			ColCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			ColCapsule = nullptr;
		}
	}
	else
	{
		ActorBaseMesh->SetRelativeLocationAndRotation(AtFirstActorPos, AtFirstActorRot);
		bIsHanding = false;
		ColCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ColCapsule = nullptr;
	}
}

void AClipboard::CBBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("HandIndex")))
	{
		if (bIsHanding)
		{
			CurrPageNum += 1;
			if (CurrPageNum == 5)
			{
				CurrPageNum = 0;
				SM_CB_Paper->SetMaterial(0, MI_StageSpritesArrs[CurrPageNum]);
			}
			else
			{
				SM_CB_Paper->SetMaterial(0, MI_StageSpritesArrs[CurrPageNum]);
			}
		}
	}
}
