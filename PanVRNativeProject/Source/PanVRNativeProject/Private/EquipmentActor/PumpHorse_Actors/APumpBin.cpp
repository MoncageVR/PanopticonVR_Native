#include "EquipmentActor/PumpHorse_Actors/APumpBin.h"
#include "MainActor/CPumpHorse.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "Kismet/KismetMathLibrary.h"

AAPumpBin::AAPumpBin()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_BinBody(TEXT("/Game/VRContent/Modeling/17_PumpAndPumpHorse/Pump/SM_PumpBin.SM_PumpBin"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_DeloreanLever(TEXT("/Game/VRContent/Modeling/17_PumpAndPumpHorse/Pump/SM_DeLoreanLever.SM_DeLoreanLever"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));

	if (ActorBaseMesh && ModelingFinder_BinBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_BinBody.Object);
	}

	SMDeLoreanLever = CreateDefaultSubobject<UStaticMeshComponent>("LeverSMComp");
	if (SMDeLoreanLever && ModelingFinder_DeloreanLever.Succeeded())
	{
		SMDeLoreanLever->SetupAttachment(ActorBaseMesh);
		SMDeLoreanLever->SetStaticMesh(ModelingFinder_DeloreanLever.Object);
		SMDeLoreanLever->SetRelativeLocation(FVector(0.f, -16.f, 0.f));
	}

	if (GC)
	{
		GC->SetupAttachment(SMDeLoreanLever);
		GC->SetRelativeLocation(FVector(0.f, -3.f, 3.f));
		GC->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		GC->EEGrabType = EGrabType::HandToObj;
	}

	SCMoveStandard = CreateDefaultSubobject<USceneComponent>("MoveStandardSceneComp");
	if (SCMoveStandard)
	{
		SCMoveStandard->SetupAttachment(SMDeLoreanLever);
		SCMoveStandard->SetRelativeLocation(FVector(0.f, 0.f, 3.f));
	}

	CLRemover = CreateDefaultSubobject<UBoxComponent>("BoxCollisionComp");
	if (CLRemover)
	{
		CLRemover->SetupAttachment(ActorBaseMesh);
		CLRemover->SetRelativeLocation(FVector(24.0f, 0.f, 3.f));
		CLRemover->SetRelativeRotation(FRotator(-17.5f, 0.f, 0.f));
		CLRemover->SetBoxExtent(FVector(3.f, 9.f, 5.f));
		CLRemover->SetHiddenInGame(false); // Debug
		CLRemover->OnComponentBeginOverlap.AddDynamic(this, &AAPumpBin::OverlapTrashBoxBegin);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_TrashNumFont(TEXT("/Game/VRContent/Font/MI_Panopticon_Font.MI_Panopticon_Font"));
	TRTrashNum = CreateDefaultSubobject<UTextRenderComponent>("TRComp");
	if (TRTrashNum)
	{
		TRTrashNum->SetupAttachment(ActorBaseMesh);
		TRTrashNum->SetRelativeLocation(FVector(1.0f, 24.3f, 5.0f));
		TRTrashNum->SetRelativeRotation(FRotator(21.0f, -107.f, 0.f));

		TRTrashNum->SetTextRenderColor(FColor::Red);
		TRTrashNum->SetWorldSize(10.f);
		TRTrashNum->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		TRTrashNum->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
		if (MatFinder_TrashNumFont.Succeeded())
			TRTrashNum->SetMaterial(0, MatFinder_TrashNumFont.Object);
	}

	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
		SMDeLoreanLever->SetMaterial(0, MaterialFinder_Main.Object);
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

void AAPumpBin::BeginPlay()
{
	Super::BeginPlay();
	this->EquipmentRegistrable(this);
	RemoveTrashNum = 0;
	// RemoveTrashNum = 4; // Debug

	TRTrashNum->SetText(FText::FromString(FString::FromInt(RemoveTrashNum)));
}

void AAPumpBin::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);
}

void AAPumpBin::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}

void AAPumpBin::OverlapTrashBoxBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const bool bIsTrash = (OtherComp && OtherComp->ComponentHasTag(FName("Trash"))) || (OtherActor && OtherActor->ActorHasTag(FName("Trash")));
	if (!bIsTrash) return;
	if (!IsValid(OtherActor)) return;

	if (OtherActor->GetAttachParentActor() != nullptr)
	{
		OtherActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	OtherActor->Destroy();
	RemoveTrashNum++;

	if (RemoveTrashNum >= 5)
	{
		TRTrashNum->SetWorldSize(5.0f);
		TRTrashNum->SetText(FText::FromString(TEXT("FULL")));
	}
	else
	{
		TRTrashNum->SetWorldSize(10.0f);
		TRTrashNum->SetText(FText::FromString(FString::FromInt(RemoveTrashNum)));
	}
}

void AAPumpBin::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, AVRHand* InGrabbingHand)
{
	TempMC = &InMCRef;
	if (RemoveTrashNum >= 4)
	{
		HVRSoundPlayer::PlaySoundEffect(this, SFX_LightGrab, this->GetRootComponent()->GetComponentLocation());
		if (UWorld* mWorld = GetWorld())
		{
			mWorld->GetTimerManager().SetTimer(
				DeLoreanLeverMoveTimer,
				this,
				&AAPumpBin::UpdateDeLoreanLeverMove,
				0.01f,
				true
			);
		}
	}
}

void AAPumpBin::OnDropped()
{
	TempMC = nullptr;
	if (UWorld* mWorld = GetWorld())
	{
		mWorld->GetTimerManager().ClearTimer(DeLoreanLeverMoveTimer);
	}
	SMDeLoreanLever->SetRelativeLocation(FVector(0.f, -16.f, 0.f));
}

void AAPumpBin::UpdateDeLoreanLeverMove()
{
	if (RemoveTrashNum >= 4)
	{
		FTransform Between_MCAndStandard_MakeRelativeTransformVar = TempMC->GetComponentTransform().GetRelativeTransform(SCMoveStandard->GetComponentTransform());

		float DeLoreanLeverMovementDir = (SMDeLoreanLever->GetRelativeLocation().Y) + (Between_MCAndStandard_MakeRelativeTransformVar.GetLocation().Y);

		float TargetPosY = FMath::Clamp(DeLoreanLeverMovementDir, -16.0f, 8.0f);

		SMDeLoreanLever->SetRelativeLocation(FVector(0.f, TargetPosY, 0.f));

		float CheckOperationResult = FMath::GetMappedRangeValueClamped(FVector2D(-16.0f, 8.0f), FVector2D(0.0f, 100.0f), TargetPosY);

		if (FMath::IsNearlyEqual(CheckOperationResult, 100.0f))
		{
			RemoveTrashNum = 0;
			TRTrashNum->SetText(FText::FromString(FString::FromInt(RemoveTrashNum)));
			this->PumpOperation();
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("%f"), CheckOperationResult);
}

void AAPumpBin::PumpOperation()
{
	EquipmentWorldSubSystem->NotifyPumpOperationBroadCast(10.0f);
	SMDeLoreanLever->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (UWorld* mWorld = GetWorld())
	{
		mWorld->GetTimerManager().SetTimer(
			PumpOperationTimer,
			this,
			&AAPumpBin::PumpOperationStop,
			10.0f,
			false
		);
	}

	UE_LOG(LogTemp, Log, TEXT("Spawn Horse Character Part!"));
	SpawnPumpHorse = this->GetWorld()->SpawnActor<ACPumpHorse>(ACPumpHorse::StaticClass(),
		FTransform(
			FRotator(0.f, 180.0f, 0.f),
			FVector(-400.0f, 0.0f, 150.15f),
			FVector::OneVector
		)
	);
}

void AAPumpBin::PumpOperationStop()
{
	EquipmentWorldSubSystem->NotifyPumpOperationBroadCast(0.0f);

	SpawnPumpHorse->HandleMoveEndAndDestroySelf();

	SMDeLoreanLever->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (UWorld* MyWorld = GetWorld())
		MyWorld->GetTimerManager().ClearTimer(PumpOperationTimer);
}
