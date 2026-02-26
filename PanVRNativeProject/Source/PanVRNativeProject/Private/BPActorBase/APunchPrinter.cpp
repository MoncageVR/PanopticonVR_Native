// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/APunchPrinter.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "BPActorBase/APaper.h"

AAPunchPrinter::AAPunchPrinter()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_PPBody(TEXT("/Game/VRContent/Modeling/Printer_Fax_Stamp/PP_Body.PP_Body"));
	if (ActorBaseMesh && ModelingFinder_PPBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_PPBody.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_PPSandbag(TEXT("/Game/VRContent/Modeling/Printer_Fax_Stamp/PP_Sandbag.PP_Sandbag"));
	PPSandbag = CreateDefaultSubobject<UStaticMeshComponent>("SM_Sandbag");
	if (PPSandbag && ModelingFinder_PPSandbag.Succeeded())
	{
		PPSandbag->SetupAttachment(ActorBaseMesh);
		PPSandbag->SetStaticMesh(ModelingFinder_PPSandbag.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_PP(TEXT("/Game/VRContent/Material/SRS_Stage_PunchPrinter.SRS_Stage_PunchPrinter"));
	if (MaterialFinder_PP.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_PP.Object);
		PPSandbag->SetMaterial(0, MaterialFinder_PP.Object);
	}

	CLPPSandbag = CreateDefaultSubobject<UBoxComponent>("CL_PPSandbag");
	if (CLPPSandbag)
	{
		CLPPSandbag->SetupAttachment(PPSandbag);
		CLPPSandbag->SetRelativeLocation(FVector(0.f, 4.f, 20.7f));
		CLPPSandbag->SetRelativeRotation(FRotator(0.f, 0.f, -8.f));
		CLPPSandbag->SetBoxExtent(FVector(8.0f, 5.25f, 11.0f));
		CLPPSandbag->SetHiddenInGame(false); // Debug
	}

	PPMovementTimeline = CreateDefaultSubobject<UTimelineComponent>("PP_TimelineComp");
	PPMovementTimeline->SetLooping(false);
	PPMovementTimeline->SetTimelineLength(1.01f);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_PPMoveCurve(TEXT("/Game/VRContent/Blueprints/TimelineCurve/PPHead_MoveCurve.PPHead_MoveCurve"));
	if (CurveFinder_PPMoveCurve.Succeeded())
	{
		PPMoveCurve = CurveFinder_PPMoveCurve.Object;
	}

	CLPPPaper = CreateDefaultSubobject<UBoxComponent>("CL_PP_Paper");
	if (CLPPPaper)
	{
		CLPPPaper->SetupAttachment(ActorBaseMesh);
		CLPPPaper->SetRelativeLocation(FVector(0.f, -5.f, -13.f));
		CLPPPaper->SetBoxExtent(FVector(10.5f, 15.f, 1.f));
		CLPPPaper->OnComponentBeginOverlap.AddDynamic(this, &AAPunchPrinter::CLPPEntranceOverlapBegin);
		CLPPPaper->OnComponentEndOverlap.AddDynamic(this, &AAPunchPrinter::CLPPEntranceFOverlapEnd);
		CLPPPaper->SetHiddenInGame(false); // Debug
	}
}

void AAPunchPrinter::BeginPlay()
{
	Super::BeginPlay();

	DefaultSpawnPaper();

	if (CLPPSandbag) // PP Punch Collision Function Binding
	{
		CLPPSandbag->OnComponentBeginOverlap.AddDynamic(this, &AAPunchPrinter::CLPPOverlapBegin);
		CLPPSandbag->OnComponentEndOverlap.AddDynamic(this, &AAPunchPrinter::CLPPFOverlapEnd);
	}

	FOnTimelineFloat ChangeValue;
	ChangeValue.BindUFunction(this, FName("PPMoveTLFunc"));
	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindUFunction(this, FName("PPMoveTLEndFunc"));

	PPMovementTimeline->AddInterpFloat(PPMoveCurve, ChangeValue);
	PPMovementTimeline->SetTimelineFinishedFunc(FinishedEvent);
}

void AAPunchPrinter::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);

	if (PPMovementTimeline)
		PPMovementTimeline->TickComponent(DeltaTimes, ELevelTick::LEVELTICK_TimeOnly, nullptr);
}

void AAPunchPrinter::CLPPOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("HandCLBox")))
	{
		if (NewSpawnPaper)
		{
			if (NewSpawnPaper->GetRootComponent()->GetRelativeLocation().Y >= 10.5f)
			{
				return;
			}
			else
			{
				PPMovementTimeline->PlayFromStart();
				CLPPSandbag->SetGenerateOverlapEvents(false);
				//NewSpawnPaper->GetRootComponent()->SetRelativeLocation(NewSpawnPaper->GetRootComponent()->GetRelativeLocation() + FVector(0.f, 1.5f, 0.f));
				NewSpawnPaper->GetRootComponent()->SetRelativeLocation(FVector(0.f, 10.5f, -13.3f));
			}
		}
		else
		{
			return;
		}
	}
}

void AAPunchPrinter::CLPPFOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

/*

*/

void AAPunchPrinter::CLPPEntranceOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AAPunchPrinter::CLPPEntranceFOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName("Paper")))
	{
		if (NewSpawnPaper == OtherActor)
		{
			NewSpawnPaper = nullptr;

			GetWorldTimerManager().SetTimer(
				NewSpawnTimer,
				this,
				&AAPunchPrinter::DefaultSpawnPaper,
				0.2f,
				false
			);
		}

	}
}


void AAPunchPrinter::PPMoveTLFunc(float Value)
{
	PPSandbag->SetRelativeRotation(FRotator(0.f, 0.f, (Value * 10)));
}

void AAPunchPrinter::PPMoveTLEndFunc()
{
	CLPPSandbag->SetGenerateOverlapEvents(true);
	PPMoveEndFunc();
}

void AAPunchPrinter::PPMoveEndFunc()
{
	if (NewSpawnPaper)
	{
		if (NewSpawnPaper->GetRootComponent()->GetRelativeLocation().Y >= 10.5f)
		{
			NewSpawnPaper->SetCanGrabPhysics(1);
		}
	}
}

void AAPunchPrinter::DefaultSpawnPaper()
{
	if (!NewSpawnPaper)
	{
		NewSpawnPaper = GetWorld()->SpawnActor<AAPaper>(
			ActorBaseMesh->GetComponentLocation(),
			ActorBaseMesh->GetComponentRotation()
		);
	}
	if (IsValid(NewSpawnPaper))
	{
		NewSpawnPaper->AttachToComponent(ActorBaseMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
		NewSpawnPaper->GetRootComponent()->SetRelativeLocation(FVector(0.f, 1.5f, -13.3f));

		NewSpawnPaper->SetCanGrabPhysics(0);

		GetWorldTimerManager().PauseTimer(NewSpawnTimer);
		GetWorldTimerManager().ClearTimer(NewSpawnTimer);
	}
	else
	{
		return;
	}
}
