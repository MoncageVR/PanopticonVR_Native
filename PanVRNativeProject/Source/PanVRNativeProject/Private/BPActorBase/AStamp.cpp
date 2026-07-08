// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/AStamp.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "BPActorBase/APaper.h"

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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Body(TEXT("/Game/VRContent/Modeling/Printer_Fax_Stamp/StampPlate.StampPlate"));

	if (ActorBaseMesh && ModelingFinder_Body.Succeeded())
	{
		ActorBaseMesh->SetupAttachment(STMainRoot);
		ActorBaseMesh->SetStaticMesh(ModelingFinder_Body.Object);
	}

	CLPaperTarget = CreateDefaultSubobject<UBoxComponent>("PaperTarget_Col");
	if (CLPaperTarget)
	{
		CLPaperTarget->SetupAttachment(ActorBaseMesh);
		CLPaperTarget->SetRelativeLocation(FVector(0.f, 0.f, 0.7f));
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Handle(TEXT("/Game/VRContent/Modeling/Printer_Fax_Stamp/StampHeadUpper.StampHeadUpper"));
	SMStampHandle = CreateDefaultSubobject<UStaticMeshComponent>("SM_StampHandle");
	if (SMStampHandle && ModelingFinder_Handle.Succeeded())
	{
		SMStampHandle->SetupAttachment(STHandleRoot);
		SMStampHandle->SetStaticMesh(ModelingFinder_Handle.Object);
		if (GC)
		{
			GC->SetupAttachment(SMStampHandle);
			GC->SetRelativeLocation(FVector(0.f, 0.f, 5.5f));
			GC->EEGrabType = EGrabType::HandToObj;
		}

	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Head(TEXT("/Game/VRContent/Modeling/Printer_Fax_Stamp/StampHeadBelow.StampHeadBelow"));
	SMStampHandleHead = CreateDefaultSubobject<UStaticMeshComponent>("SM_StampHandleHead");
	if (SMStampHandleHead && ModelingFinder_Head.Succeeded())
	{
		SMStampHandleHead->SetupAttachment(SMStampHandle);
		SMStampHandleHead->SetStaticMesh(ModelingFinder_Head.Object);
	}

	CLStamp = CreateDefaultSubobject<UBoxComponent>("Stamp_Col");
	if (CLStamp)
	{
		CLStamp->SetupAttachment(SMStampHandleHead);
		CLStamp->SetRelativeLocation(FVector(0.f, 0.f, 3.f));
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
		GC->SetRelativeLocationAndRotation(FVector(0.f, 4.f, 13.f), FRotator(0.f, 180.f, 20.f));
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
