// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/AABButton.h"
#include "Components/BoxComponent.h"
#include "VRPawn/VRCharacterBase.h"

AAABButton::AAABButton()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setting SceneRoot
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	if (DefaultSceneRoot)
	{
		SetRootComponent(DefaultSceneRoot);
	}

	// Setting AB Button Plate 
	ActorBaseMesh->SetupAttachment(DefaultSceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ABbuttonPlate(TEXT("/Game/VRContent/Modeling/ABbutton/AB_Button_Plate.AB_Button_Plate"));
	if (ModelingFinder_ABbuttonPlate.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_ABbuttonPlate.Object);
	}
	ActorBaseMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	// Setting A And B Button Mesh
	ButtonA = CreateDefaultSubobject<UStaticMeshComponent>("SM_ButtonA");
	if (ButtonA)
	{
		ButtonA->SetupAttachment(ActorBaseMesh);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ButtonA(TEXT("/Game/VRContent/Modeling/ABbutton/AB_Button_A_Button.AB_Button_A_Button"));
	if (ModelingFinder_ButtonA.Succeeded())
	{
		ButtonA->SetStaticMesh(ModelingFinder_ButtonA.Object);
	}

	//ButtonB
	ButtonB = CreateDefaultSubobject<UStaticMeshComponent>("SM_ButtonB");
	if (ButtonB)
	{
		ButtonB->SetupAttachment(ActorBaseMesh);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ButtonB(TEXT("/Game/VRContent/Modeling/ABbutton/AB_Button_B_Button.AB_Button_B_Button"));
	if (ModelingFinder_ButtonB.Succeeded())
	{
		ButtonB->SetStaticMesh(ModelingFinder_ButtonB.Object);
	}

	// Setting A And B Collision 
	FVector CLRelativePos(0.0f, 13.5f, 7.0f);
	FRotator CLRelativeRot(0.0f, 90.0f, 0.0f);
	FVector CLExtentVec(7.0f, 7.0f, 3.0f);

	CLButtonA = CreateDefaultSubobject<UBoxComponent>("ABoxCollisionComponent");
	if (CLButtonA != nullptr)
	{
		CLButtonA->SetupAttachment(DefaultSceneRoot);
		CLButtonA->SetRelativeLocation(CLRelativePos);
		CLButtonA->SetRelativeRotation(CLRelativeRot);
		CLButtonA->SetBoxExtent(CLExtentVec);
		CLButtonA->SetHiddenInGame(false); // Debug
		CLButtonA->SetGenerateOverlapEvents(true);
	}

	CLButtonB = CreateDefaultSubobject<UBoxComponent>("BBoxCollisionComponent");
	if (CLButtonB != nullptr)
	{
		CLButtonB->SetupAttachment(DefaultSceneRoot);
		CLButtonB->SetRelativeLocation(FVector(CLRelativePos.X, -CLRelativePos.Y, CLRelativePos.Z));
		CLButtonB->SetRelativeRotation(CLRelativeRot);
		CLButtonB->SetBoxExtent(CLExtentVec);
		CLButtonB->SetHiddenInGame(false); // Debug
		CLButtonB->SetGenerateOverlapEvents(true);
	}

	CLButtonA->OnComponentBeginOverlap.AddDynamic(this, &AAABButton::OverlapABoxBegin);
	CLButtonA->OnComponentEndOverlap.AddDynamic(this, &AAABButton::OverlapABoxEnd);

	CLButtonB->OnComponentBeginOverlap.AddDynamic(this, &AAABButton::OverlapBBoxBegin);
	CLButtonB->OnComponentEndOverlap.AddDynamic(this, &AAABButton::OverlapBBoxEnd);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_ABButton(TEXT("/Game/VRContent/Material/SRS_STAGE_ABbutton.SRS_STAGE_ABbutton"));
	if (MaterialFinder_ABButton.Succeeded())
	{
		ButtonA->SetMaterial(0, MaterialFinder_ABButton.Object);
		ButtonB->SetMaterial(0, MaterialFinder_ABButton.Object);
	}

	//if()
}

void AAABButton::BeginPlay()
{
	Super::BeginPlay();

	//EquipmentRegistrable(this);
}

void AAABButton::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}


void AAABButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// On Begin And End Overlap Component For Collision Button A
void AAABButton::OverlapABoxBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsButtonAPressed)
	{
		bIsButtonAPressed = 1;
	}

	if (bIsButtonAPressed)
	{
		if(OtherComp && OtherComp->ComponentHasTag(FName("HandCLBox")))
			ButtonMovement(ButtonA, FVector(0.0f, 0.0f, -2.0f));
	}

	return;
}

void AAABButton::OverlapABoxEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("HandCLBox")))
		ButtonMovement(ButtonA, FVector(0.0f, 0.0f, 0.0f));

	if (bIsButtonAPressed)
		bIsButtonAPressed = 0;

	return;
}


// On Begin And End Overlap Component For Collision Button B
void AAABButton::OverlapBBoxBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsButtonBPressed)
	{
		bIsButtonBPressed = 1;
	}

	if (bIsButtonBPressed)
	{
		if (OtherComp && OtherComp->ComponentHasTag(FName("HandCLBox")))
			ButtonMovement(ButtonB, FVector(0.0f, 0.0f, -2.0f));
	}

	return;
}

void AAABButton::OverlapBBoxEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherComp && OtherComp->ComponentHasTag(FName("HandCLBox")))
		ButtonMovement(ButtonB, FVector(0.0f, 0.0f, 0.0f));

	if (bIsButtonBPressed)
		bIsButtonBPressed = 0;

	return;
}

// Actually Static Mesh Button Moving Part Function
// Input : ( Moveing StaticMesh Comp , Target Relative Location )
void AAABButton::ButtonMovement(UStaticMeshComponent* InActuallyMoveSMButton, FVector InTargetPos)
{
	if (InActuallyMoveSMButton)
	{
		InActuallyMoveSMButton->SetRelativeLocation(InTargetPos, false, nullptr, ETeleportType::TeleportPhysics);
	}
	return;
}