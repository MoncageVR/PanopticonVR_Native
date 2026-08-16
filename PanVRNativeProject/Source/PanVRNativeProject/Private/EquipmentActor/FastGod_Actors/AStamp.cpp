#include "EquipmentActor/FastGod_Actors/AStamp.h"
#include "EquipmentActor/Spawned_Actors/APaper.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"

AAStamp::AAStamp()
{
	PrimaryActorTick.bCanEverTick = false;

	STMainRoot = CreateDefaultSubobject<USceneComponent>("StampSceneComp");
	if (STMainRoot)
	{
		SetRootComponent(STMainRoot);
	}

	STMoveRail = CreateDefaultSubobject<USplineComponent>("SP_PathComp");
	if (STMoveRail)
	{
		STMoveRail->SetupAttachment(STMainRoot);
		STMoveRail->SetTangentsAtSplinePoint(0, FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), ESplineCoordinateSpace::Local, true);
		STMoveRail->SetTangentsAtSplinePoint(1, FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), ESplineCoordinateSpace::Local, true);

		STMoveRail->SetLocationAtSplinePoint(0, FVector(0.0f, 0.0f, 0.0f), ESplineCoordinateSpace::Local, true);
		STMoveRail->SetLocationAtSplinePoint(1, FVector(0.0f, 0.0f, -2.5f), ESplineCoordinateSpace::Local, true);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Body(TEXT("/Game/VRContent/Modeling/15_Printer_Fax_Stamp/SM_StampPlate.SM_StampPlate"));

	if (ActorBaseMesh && ModelingFinder_Body.Succeeded())
	{
		ActorBaseMesh->SetupAttachment(STMainRoot);
		ActorBaseMesh->SetStaticMesh(ModelingFinder_Body.Object);
	}

	CLPaperTarget = CreateDefaultSubobject<UBoxComponent>("PaperTarget_Col");
	if (CLPaperTarget)
	{
		CLPaperTarget->SetupAttachment(ActorBaseMesh);
		CLPaperTarget->SetRelativeLocation(FVector(0.f, -5.0f, -3.7f));
		CLPaperTarget->SetBoxExtent(FVector(8.f, 3.f, 0.2f));
		CLPaperTarget->SetHiddenInGame(false); // Debug

		CLPaperTarget->OnComponentBeginOverlap.AddDynamic(this, &AAStamp::CLPaperTargetOverlapBegin);
		CLPaperTarget->OnComponentEndOverlap.AddDynamic(this, &AAStamp::CLPaperTargetOverlapEnd);
	}

	STHandleRoot = CreateDefaultSubobject<USceneComponent>("HandleSceneComp");
	if (STHandleRoot)
	{
		STHandleRoot->SetupAttachment(STMainRoot);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_HeadUpper(TEXT("/Game/VRContent/Modeling/15_Printer_Fax_Stamp/SM_StampHead_Upper.SM_StampHead_Upper"));
	SMStampHandle = CreateDefaultSubobject<UStaticMeshComponent>("SM_StampHandle");
	if (SMStampHandle && ModelingFinder_HeadUpper.Succeeded())
	{
		SMStampHandle->SetupAttachment(STHandleRoot);
		SMStampHandle->SetStaticMesh(ModelingFinder_HeadUpper.Object);
		if (GC)
		{
			GC->SetupAttachment(SMStampHandle);
			GC->SetRelativeLocation(FVector(0.f, 0.f, 2.5f));
			GC->EEGrabType = EGrabType::HandToObj;
		}

	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_HeadBelow(TEXT("/Game/VRContent/Modeling/15_Printer_Fax_Stamp/SM_StampHead_Below.SM_StampHead_Below"));
	SMStampHandleHead = CreateDefaultSubobject<UStaticMeshComponent>("SM_StampHandleHead");
	if (SMStampHandleHead && ModelingFinder_HeadBelow.Succeeded())
	{
		SMStampHandleHead->SetupAttachment(SMStampHandle);
		SMStampHandleHead->SetStaticMesh(ModelingFinder_HeadBelow.Object);
	}

	CLStamp = CreateDefaultSubobject<UBoxComponent>("Stamp_Col");
	if (CLStamp)
	{
		CLStamp->SetupAttachment(SMStampHandleHead);
		CLStamp->SetRelativeLocation(FVector(0.f, -5.1f, -1.7f));
		CLStamp->SetBoxExtent(FVector(3.8f, 2.0f, 1.0f));
		//CLStamp->SetHiddenInGame(false); // Debug
		CLStamp->OnComponentBeginOverlap.AddDynamic(this, &AAStamp::CLStampOverlapBegin);

		CLStamp->SetGenerateOverlapEvents(false);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
		SMStampHandle->SetMaterial(0, MaterialFinder_Main.Object);
		SMStampHandleHead->SetMaterial(0, MaterialFinder_Main.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_Move(TEXT("/Game/VRContent/Sound/Wavs/FastGod/sfx_stamp_move.sfx_stamp_move"));
	if (SoundFinder_Move.Succeeded())
	{
		SFXStampMove = SoundFinder_Move.Object;
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

void AAStamp::BeginPlay()
{
	Super::BeginPlay();
}

void AAStamp::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);
}

void AAStamp::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, AVRHand* InGrabbingHand)
{
	AdjustGCPosNRot(1);

	TempMCRef = &InMCRef;

	GetWorldTimerManager().SetTimer(
		StampMoveTimer,
		this,
		&AAStamp::MoveStamp,
		0.01f,
		true
	);
}

void AAStamp::OnDropped()
{
	AdjustGCPosNRot(0);

	if (TempMCRef)
		TempMCRef = nullptr;

	if (GetWorldTimerManager().IsTimerActive(StampMoveTimer))
	{
		GetWorldTimerManager().PauseTimer(StampMoveTimer);
		GetWorldTimerManager().ClearTimer(StampMoveTimer);
	}

	SMStampHandle->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

void AAStamp::MoveStamp()
{
	float FinalPosZ = FMath::Clamp(
		(SMStampHandle->GetRelativeLocation().Z + (TempMCRef->GetComponentTransform().GetRelativeTransform(STHandleRoot->GetComponentTransform()).GetLocation().Z)),
		STMoveRail->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local).Z,
		STMoveRail->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local).Z
	);

	SMStampHandle->SetRelativeLocation(FVector(0.f, 0.f, FinalPosZ));
}

void AAStamp::AdjustGCPosNRot(uint32 InFlag)
{
	if (InFlag)
		GC->SetRelativeLocationAndRotation(FVector(0.f, 4.f, 10.f), FRotator(0.f, 180.f, 20.f));
	else
		GC->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
}

void AAStamp::CLPaperTargetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!AttachingPaper)
	{
		if (OtherComp->ComponentHasTag(FName("Paper")))
		{
			if (IsValid(Cast<AAPaper>(OtherActor)))
			{
				AttachingPaper = Cast<AAPaper>(OtherActor);

				if (AttachingPaper)
				{
					AttachingPaper->SetIsStampAttaching(1);
					AttachingPaper->GC->GCTryRelease();
					AttachingPaper->GC->SetPrimitiveCompPhysics(false);
					AttachingPaper->AttachToComponent(CLPaperTarget, FAttachmentTransformRules::SnapToTargetIncludingScale);

					CLStamp->SetGenerateOverlapEvents(true);
					CLStamp->SetHiddenInGame(false); // Debug
				}
			}
		}
	}
}

void AAStamp::CLPaperTargetOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName("Paper")))
	{
		if (AttachingPaper == Cast<AAPaper>(OtherActor))
		{
			AttachingPaper->SetIsStampAttaching(0);

			CLStamp->SetGenerateOverlapEvents(false);
			CLStamp->SetHiddenInGame(true); // Debug

			AttachingPaper = nullptr;
		}
	}
}

void AAStamp::CLStampOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("Paper")))
	{
		if (!AttachingPaper->GetIsStamping())
		{
			mSoundPlayer->PlaySoundEffect(this, SFXStampMove, ActorBaseMesh->GetComponentLocation());
			AttachingPaper->StampOn();
		}
	}
}
