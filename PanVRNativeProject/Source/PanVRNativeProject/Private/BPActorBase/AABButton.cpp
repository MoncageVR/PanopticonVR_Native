// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/AABButton.h"
#include "Components/BoxComponent.h"
#include "VRPawn/VRCharacterBase.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"

AAABButton::AAABButton()
{
	PrimaryActorTick.bCanEverTick = true;

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

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_A(TEXT("/Game/VRContent/Sound/Wavs/ABButton/sfx_ab_a.sfx_ab_a"));
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_B(TEXT("/Game/VRContent/Sound/Wavs/ABButton/sfx_ab_b.sfx_ab_b"));
	if (SoundFinder_A.Succeeded() && SoundFinder_B.Succeeded())
	{
		AButtonSFX = SoundFinder_A.Object;
		BButtonSFX = SoundFinder_B.Object;
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

void AAABButton::BeginPlay()
{
	Super::BeginPlay();

	this->EquipmentRegistrable(this);
}

void AAABButton::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}


void AAABButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AnswerSaveList.Num() == 4)
	{
		CheckCorrectAnswer();
	}
}

// On Begin And End Overlap Component For Collision Button A
void AAABButton::OverlapABoxBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetWorldTimerManager().IsTimerActive(NextAnswerCheckTimer))
	{
		GetWorldTimerManager().PauseTimer(NextAnswerCheckTimer);
		GetWorldTimerManager().ClearTimer(NextAnswerCheckTimer);
	}

	if (!bIsButtonAPressed)
	{
		bIsButtonAPressed = 1;
	}

	if (bIsButtonAPressed)
	{
		if (OtherComp && OtherComp->ComponentHasTag(FName("HandCLBox")))
		{
			ButtonMovement(ButtonA, FVector(0.0f, 0.0f, -2.0f));
			mSoundPlayer->PlaySoundEffect(this, AButtonSFX, ButtonA->GetComponentLocation());
			AnswerSaveList.Add(1);
		}
	}

	return;
}

void AAABButton::OverlapABoxEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("HandCLBox")))
		ButtonMovement(ButtonA, FVector(0.0f, 0.0f, 0.0f));

	if (bIsButtonAPressed)
		bIsButtonAPressed = 0;

	GetWorldTimerManager().SetTimer(
		NextAnswerCheckTimer, 
		this,
		&AAABButton::NextAnswerNonInputClear,
		1.5f,
		false);

	return;
}


// On Begin And End Overlap Component For Collision Button B
void AAABButton::OverlapBBoxBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetWorldTimerManager().IsTimerActive(NextAnswerCheckTimer))
	{
		GetWorldTimerManager().PauseTimer(NextAnswerCheckTimer);
		GetWorldTimerManager().ClearTimer(NextAnswerCheckTimer);
	}

	if (!bIsButtonBPressed)
	{
		bIsButtonBPressed = 1;
	}

	if (bIsButtonBPressed)
	{
		if (OtherComp && OtherComp->ComponentHasTag(FName("HandCLBox")))
		{
			ButtonMovement(ButtonB, FVector(0.0f, 0.0f, -2.0f));
			mSoundPlayer->PlaySoundEffect(this, BButtonSFX, ButtonB->GetComponentLocation());
			AnswerSaveList.Add(0);
		}
	}

	return;
}

void AAABButton::OverlapBBoxEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherComp && OtherComp->ComponentHasTag(FName("HandCLBox")))
		ButtonMovement(ButtonB, FVector(0.0f, 0.0f, 0.0f));

	if (bIsButtonBPressed)
		bIsButtonBPressed = 0;

	GetWorldTimerManager().SetTimer(
		NextAnswerCheckTimer,
		this,
		&AAABButton::NextAnswerNonInputClear,
		1.5f,
		false);

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

void AAABButton::NextAnswerNonInputClear()
{
	if (AnswerSaveList.IsEmpty())
		return;
	else
		AnswerSaveList.Empty();
}

void AAABButton::CheckCorrectAnswer()
{
	TArray<FString> TempAnswerSaveStringList;

	for (uint8 index : AnswerSaveList)
	{
		TempAnswerSaveStringList.Add(FString::FromInt(index));
	}

	FString Result = FString::Join(TempAnswerSaveStringList, TEXT(""));

	if (Result == CorrectAnswerFirstCase)
	{
		if (IsValid(EquipmentWorldSubSystem))
		{
			EquipmentWorldSubSystem->NotifyGloveOperateBroadCast();
		}
		CoolDownApplyFunc(0);
	}
	else if (Result == CorrectAnswerSecondCase)
	{
		// TopEscape Subdue Function Call Part!
		CoolDownApplyFunc(0);
	}
	else
	{
		// Spawn Pickle Function Call Part!
		CoolDownApplyFunc(1);
	}

	TempAnswerSaveStringList.Empty();
	AnswerSaveList.Empty();
}

void AAABButton::CoolDownApplyFunc(uint8 InCoolTimeFlag)
{
	CLButtonA->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CLButtonB->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ButtonMovement(ButtonA, FVector(0.0f, 0.0f, -2.0f));
	ButtonMovement(ButtonB, FVector(0.0f, 0.0f, -2.0f));

	if (InCoolTimeFlag)
	{
		GetWorldTimerManager().SetTimer(
			CooldownApplyTimer,
			this,
			&AAABButton::CoolDownUnApplyFunc,
			2.0f,
			false
		);
	}
	else
	{
		GetWorldTimerManager().SetTimer(
			CooldownApplyTimer,
			this,
			&AAABButton::CoolDownUnApplyFunc,
			10.0f,
			false
		);
	}
}

void AAABButton::CoolDownUnApplyFunc()
{
	ButtonMovement(ButtonA, FVector(0.0f, 0.0f, 0.0f));
	ButtonMovement(ButtonB, FVector(0.0f, 0.0f, 0.0f));

	CLButtonA->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CLButtonB->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	if (GetWorldTimerManager().IsTimerActive(CooldownApplyTimer))
	{
		GetWorldTimerManager().PauseTimer(CooldownApplyTimer);
		GetWorldTimerManager().ClearTimer(CooldownApplyTimer);
	}
}
