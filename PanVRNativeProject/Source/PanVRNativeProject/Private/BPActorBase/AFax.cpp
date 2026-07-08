// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/AFax.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "BPActorBase/APaper.h"

AAFax::AAFax()
{
	PrimaryActorTick.bCanEverTick = false;

	FaxSceneRoot = CreateDefaultSubobject<USceneComponent>("FaxSceneComp");
	if (FaxSceneRoot)
	{
		SetRootComponent(FaxSceneRoot);
	}

	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetupAttachment(FaxSceneRoot);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_FaxBody(TEXT("/Game/VRContent/Modeling/Printer_Fax_Stamp/FAXBody.FAXBody"));
	if (ActorBaseMesh && ModelingFinder_FaxBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_FaxBody.Object);
	}

	FaxBodyCol = CreateDefaultSubobject<UBoxComponent>("CL_FaxBody");
	if (FaxBodyCol)
	{
		FaxBodyCol->SetupAttachment(ActorBaseMesh);
		FaxBodyCol->SetRelativeLocation(FVector(4.95f, -0.8f, 3.0f));
		FaxBodyCol->SetBoxExtent(FVector(0.7f, 6.5f, 1.f));
		FaxBodyCol->SetHiddenInGame(false); // Debug
	}

	FaxButtonCol = CreateDefaultSubobject<UCapsuleComponent>("CL_FaxButton");
	if (FaxButtonCol)
	{
		FaxButtonCol->SetupAttachment(ActorBaseMesh);
		FaxButtonCol->SetRelativeLocation(FVector(1.4f, -4.2f, 3.5f));
		FaxButtonCol->SetCapsuleRadius(2.3f);
		FaxButtonCol->SetCapsuleHalfHeight(2.3f);
		FaxButtonCol->SetHiddenInGame(false); // Debug
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
	}

	FaxBodyCol->OnComponentBeginOverlap.AddDynamic(this, &AAFax::FaxColOverlapBegin);
	FaxButtonCol->OnComponentBeginOverlap.AddDynamic(this, &AAFax::FaxButtonColOverlapBegin);

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_Success(TEXT("/Game/VRContent/Sound/Wavs/FastGod/sfx_fax_buttonsuccess.sfx_fax_buttonsuccess"));
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_Fail(TEXT("/Game/VRContent/Sound/Wavs/FastGod/sfx_fax_buttonfail.sfx_fax_buttonfail"));

	if (SoundFinder_Success.Succeeded() && SoundFinder_Fail.Succeeded())
	{
		SFXShredSuccess = SoundFinder_Success.Object;
		SFXShredFail = SoundFinder_Fail.Object;
	}

	//SFXShredSuccess

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

void AAFax::FaxColOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!AttachingPaper)
	{
		if (OtherComp->ComponentHasTag(FName("Paper")))
		{
			if (IsValid(AttachingPaper = Cast<AAPaper>(OtherActor)))
			{
				if (AttachingPaper->GetIsPrinting() && AttachingPaper->GetIsStamping())
				{
					AttachingPaper->SetIsFaxAttaching(1);
					AttachingPaper->GC->GCTryRelease();
					AttachingPaper->GC->SetPrimitiveCompPhysics(false);
					AttachingPaper->AttachToComponent(FaxBodyCol, FAttachmentTransformRules::SnapToTargetIncludingScale);
					AttachingPaper->GetRootComponent()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 7.5f), FRotator(0.f, 90.f, 90.f));
					AttachingPaper->SetCanGrabPhysics(0);

					AttachedPaperZPos = AttachingPaper->GetRootComponent()->GetComponentLocation().Z;
				}
				else
				{
					AttachingPaper = nullptr;
				}
			}
		}
	}
}

void AAFax::FaxButtonColOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("HandIndex")) && AttachingPaper)
	{
		if (AttachingPaper->GetIsFaxAttaching())
		{
			mSoundPlayer->PlaySoundEffect(this, SFXShredSuccess, ActorBaseMesh->GetComponentLocation());
			GetWorldTimerManager().SetTimer(
				FaxOperationTimer,
				this,
				&AAFax::PaperShredding,
				0.01f,
				true
			);

			AttachingPaper->SetIsFaxAttaching(0);
			FaxButtonCol->SetHiddenInGame(true); // Debug
			FaxButtonCol->SetGenerateOverlapEvents(false);
		}
	}

	if (OtherComp->ComponentHasTag(FName("HandIndex")) && !AttachingPaper)
	{
		mSoundPlayer->PlaySoundEffect(this, SFXShredFail, ActorBaseMesh->GetComponentLocation());
	}
}

void AAFax::PaperShredding()
{
	if (AttachingPaper->GetRootComponent()->GetComponentLocation().Z > AttachedPaperZPos - 17.0f)
	{
		AttachingPaper->AddActorLocalOffset(FVector(0.f, 0.1f, 0.f));
	}
	else
	{
		/*
		Teleport And Floating State Subdue Event Part
		*/

		FaxButtonCol->SetGenerateOverlapEvents(true);
		FaxButtonCol->SetHiddenInGame(false); // Debug

		AttachingPaper->Destroy();
		AttachingPaper = nullptr;
		GetWorldTimerManager().PauseTimer(FaxOperationTimer);
		GetWorldTimerManager().ClearTimer(FaxOperationTimer);
	}
}
