// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/APaper.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

AAPaper::AAPaper()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Body(TEXT("/Game/VRContent/Modeling/Printer_Fax_Stamp/Paper.Paper"));
	if (ActorBaseMesh && ModelingFinder_Body.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_Body.Object);
		ActorBaseMesh->SetCollisionProfileName("PhysicsActor");
		ActorBaseMesh->SetSimulatePhysics(true);
	}

	if (GC)
	{
		GC->EEGrabType = EGrabType::Free;
	}

	CLA4 = CreateDefaultSubobject<UBoxComponent>("CL_BoxComp");
	if (CLA4)
	{
		CLA4->SetupAttachment(ActorBaseMesh);
		CLA4->SetBoxExtent(FVector(6.0f, 8.5f, 0.5f));
		CLA4->ComponentTags.Add(FName("Paper"));
		CLA4->SetHiddenInGame(false); // Debug
	}

	TR_CharPrinter = CreateDefaultSubobject<UTextRenderComponent>("TRComp");
	if (TR_CharPrinter)
	{
		TR_CharPrinter->SetupAttachment(ActorBaseMesh);
		TR_CharPrinter->SetRelativeLocation(FVector(0.f, 0.f, 0.1f));
		TR_CharPrinter->SetRelativeRotation(FRotator(90.0f, 0.0f, -90.0f));
		TR_CharPrinter->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
		TR_CharPrinter->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
		TR_CharPrinter->TextRenderColor = FColor::Red;
		TR_CharPrinter->SetXScale(1.0f);
		TR_CharPrinter->SetYScale(1.0f);
		TR_CharPrinter->SetWorldSize(5.0f);
		TR_CharPrinter->Text = FText();
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Paper(TEXT("/Game/VRContent/Material/SRS_STAGE_Paper.SRS_STAGE_Paper"));
	if (MaterialFinder_Paper.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Paper.Object);
	}
}

void AAPaper::BeginPlay()
{
	Super::BeginPlay();

	ActorBaseMesh->SetSimulatePhysics(false);
	/*ActorBaseMesh->SetCollisionProfileName(FName("NoCollision"));
	CLA4->SetCollisionProfileName(FName("NoCollision"));*/
}

void AAPaper::Tick(float DeltaTimes) 
{
	Super::Tick(DeltaTimes);
}

void AAPaper::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, AVRHand* InGrabbingHand)
{
	GC->SetPrimitiveCompPhysics(false);
	GetWorldTimerManager().PauseTimer(DestroySelfTimer);
	bIsHanding = 1;
}

void AAPaper::OnDropped()
{
	bIsHanding = 0;
	ActorBaseMesh->SetCollisionProfileName(FName("PhysicsActor"));
	CLA4->SetCollisionProfileName(FName("OverlapAllDynamic"));
	GC->SetPrimitiveCompPhysics(true);

	if (!bIsHanding && !bIsPrinterAttaching && !bIsStampAttaching && !bIsFaxAttaching)
	{
		GetWorldTimerManager().SetTimer(
			DestroySelfTimer,
			this,
			&AAPaper::DestroySelf,
			3.0f,
			false
		);
	}
}

void AAPaper::DestroySelf()
{
	UE_LOG(LogTemp, Log, TEXT("Paper Destroy!"));

	GetWorldTimerManager().PauseTimer(DestroySelfTimer);
	GetWorldTimerManager().ClearTimer(DestroySelfTimer);

	this->Destroy();
}

void AAPaper::SetCanGrabPhysics(uint32 InFlag)
{
	if (InFlag)
	{
		ActorBaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CLA4->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CLA4->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	}
	else
	{
		ActorBaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CLA4->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CLA4->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	}
}

void AAPaper::StampOn()
{
	UE_LOG(LogTemp, Log, TEXT("%s is Stamp On!!!"), *GetClass()->GetName());

	bIsStamping = 1;
	
	TR_CharPrinter->SetText(FText::FromString(TEXT("PLZ")));
}

void AAPaper::SetIsPrinterAttaching(uint32 InFlag) { bIsPrinterAttaching = InFlag; }
void AAPaper::SetIsStampAttaching(uint32 InFlag) { bIsStampAttaching = InFlag; }
void AAPaper::SetIsFaxAttaching(uint32 InFlag) { bIsFaxAttaching = InFlag; }

uint32 AAPaper::GetIsPrinterAttaching() { return bIsPrinterAttaching; }
uint32 AAPaper::GetIsStampAttaching() { return bIsStampAttaching; }
uint32 AAPaper::GetIsFaxAttaching() { return bIsFaxAttaching; }
uint32 AAPaper::GetIsStamping() { return bIsStamping; }
