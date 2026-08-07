#include "EquipmentActor/AABButton.h"
#include "Components/BoxComponent.h"
#include "CoreCommon/VRPawn/VRCharacterBase.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreObj/Manager/GameInstanceSubSystem/MapObjManagerSubsystem.h"

AAABButton::AAABButton()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setting SceneRoot
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	if (DefaultSceneRoot)
	{
		SetRootComponent(DefaultSceneRoot);
	}

	// Setting AB Button Plate StaticMesh
	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetupAttachment(DefaultSceneRoot);
		ActorBaseMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ABbuttonPlate(TEXT("/Game/VRContent/Modeling/01_ABbutton/AB_Button_Plate.AB_Button_Plate"));
	if (ModelingFinder_ABbuttonPlate.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_ABbuttonPlate.Object);
	}

	// Setting A Button Static Mesh
	ButtonA = CreateDefaultSubobject<UStaticMeshComponent>("SM_ButtonA");
	if (ButtonA)
	{
		ButtonA->SetupAttachment(ActorBaseMesh);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ButtonA(TEXT("/Game/VRContent/Modeling/01_ABbutton/AB_Button_A_Button.AB_Button_A_Button"));
	if (ModelingFinder_ButtonA.Succeeded())
	{
		ButtonA->SetStaticMesh(ModelingFinder_ButtonA.Object);
	}

	// Setting B Button Static Mesh
	ButtonB = CreateDefaultSubobject<UStaticMeshComponent>("SM_ButtonB");
	if (ButtonB)
	{
		ButtonB->SetupAttachment(ActorBaseMesh);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ButtonB(TEXT("/Game/VRContent/Modeling/01_ABbutton/AB_Button_B_Button.AB_Button_B_Button"));
	if (ModelingFinder_ButtonB.Succeeded())
	{
		ButtonB->SetStaticMesh(ModelingFinder_ButtonB.Object);
	}

	CLButtonA = CreateDefaultSubobject<UBoxComponent>("ABoxCollisionComponent");
	if (CLButtonA)
	{
		CLButtonA->SetupAttachment(DefaultSceneRoot);
		CLButtonA->SetRelativeLocation(FVector(0.f, 13.5f, 7.0f));
		CLButtonA->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
		CLButtonA->SetBoxExtent(FVector(7.f, 7.f, 3.f));
		CLButtonA->SetGenerateOverlapEvents(true);
	}

	CLButtonB = CreateDefaultSubobject<UBoxComponent>("BBoxCollisionComponent");
	if (CLButtonB)
	{
		CLButtonB->SetupAttachment(DefaultSceneRoot);
		CLButtonB->SetRelativeLocation(FVector(0.f, -13.5f, 7.0f));
		CLButtonB->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
		CLButtonB->SetBoxExtent(FVector(7.f, 7.f, 3.f));
		CLButtonB->SetGenerateOverlapEvents(true);
	}

	// Each Collision Bind OnComponentBegin And End Overlap Function
	CLButtonA->OnComponentBeginOverlap.AddDynamic(this, &AAABButton::OverlapABoxBegin);
	CLButtonA->OnComponentEndOverlap.AddDynamic(this, &AAABButton::OverlapABoxEnd);
	CLButtonB->OnComponentBeginOverlap.AddDynamic(this, &AAABButton::OverlapBBoxBegin);
	CLButtonB->OnComponentEndOverlap.AddDynamic(this, &AAABButton::OverlapBBoxEnd);

	/*
	Each StaticMesh Component Material Setting
	*/
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_ABbutton.SRS_STAGE_ABbutton"));
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

	// Button A and B Sound Setting
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

// Debug Function Called from the Spectator
void AAABButton::Debug_TopEscapeStatePrisonerSubdue()
{
	//UE_LOG(LogTemp, Log, TEXT("TopEscape State Subdue!"));

	EquipmentWorldSubSystem->NotifyJailOperationControlByABBroadCast(TEXT("TopEscape"));
}


void AAABButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AnswerSaveList.Num() == 4)
	{
		CheckCorrectAnswer();
	}
}

// Begin Overlap For Collision Component Button A
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

// End Overlap For Collision Component Button A
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

// Begin Overlap For Collision Component Button B
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

// End Overlap For Collision Component Button B
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

// If there is no next Input, Clear the input Array
void AAABButton::NextAnswerNonInputClear()
{
	if (AnswerSaveList.IsEmpty())
		return;
	else
		AnswerSaveList.Empty();
}

// Answer Check Main Logic
void AAABButton::CheckCorrectAnswer()
{
	TArray<FString> TempAnswerSaveStringList;

	for (uint8 index : AnswerSaveList)
	{
		TempAnswerSaveStringList.Add(FString::FromInt(index));
	}

	FString Result = FString::Join(TempAnswerSaveStringList, TEXT(""));

	check(EquipmentWorldSubSystem);

	if (Result == CorrectAnswerFirstCase)
	{
		EquipmentWorldSubSystem->NotifyJailOperationControlByABBroadCast(TEXT("Glove"));
		CoolDownApplyFunc(0); // CoolDown 10 Second Apply
	}
	else if (Result == CorrectAnswerSecondCase)
	{
		EquipmentWorldSubSystem->NotifyJailOperationControlByABBroadCast(TEXT("TopEscape"));
		CoolDownApplyFunc(0); // CoolDown 10 Second Apply
	}
	else
	{
		//MapObjManagerGameInstSubsystemRef->HandleSpawnPickleObj();
		CoolDownApplyFunc(1); // CoolDown 2 Second Apply
	}

	TempAnswerSaveStringList.Empty();
	AnswerSaveList.Empty();
}

// Cooldown Application Function
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

// Cooldown removal Function
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
