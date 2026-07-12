// Fill out your copyright notice in the Description page of Project Settings.


#include "BPMainActorBase/JailBuilding.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"

AJailBuilding::AJailBuilding()
{
	PrimaryActorTick.bCanEverTick = true;

	JailMainRoot = CreateDefaultSubobject<USceneComponent>("MainSceneComp");
	if (JailMainRoot)
	{
		SetRootComponent(JailMainRoot);
		JailMainRoot->Mobility = EComponentMobility::Static;
	}

	FirstFloorRoot = CreateDefaultSubobject<USceneComponent>("1stFloorSceneComp");
	if (FirstFloorRoot)
	{
		FirstFloorRoot->SetupAttachment(JailMainRoot);
		FirstFloorRoot->Mobility = EComponentMobility::Movable;
	}

	SecondFloorRoot = CreateDefaultSubobject<USceneComponent>("2ndFloorSceneComp");
	if (SecondFloorRoot)
	{
		SecondFloorRoot->SetupAttachment(JailMainRoot);
		SecondFloorRoot->Mobility = EComponentMobility::Movable;
	}

	ThirdFloorRoot = CreateDefaultSubobject<USceneComponent>("3rdFloorSceneComp");
	if (ThirdFloorRoot)
	{
		ThirdFloorRoot->SetupAttachment(JailMainRoot);
		ThirdFloorRoot->Mobility = EComponentMobility::Movable;
	}

	MoveableBuildingsRoot = CreateDefaultSubobject<USceneComponent>("MoveableSceneComp");
	if (MoveableBuildingsRoot)
	{
		MoveableBuildingsRoot->SetupAttachment(JailMainRoot);
		MoveableBuildingsRoot->Mobility = EComponentMobility::Movable;
	}

	StaticBuildingsRoot = CreateDefaultSubobject<USceneComponent>("StaticSceneComp");
	if (StaticBuildingsRoot)
	{
		StaticBuildingsRoot->SetupAttachment(JailMainRoot);
		StaticBuildingsRoot->Mobility = EComponentMobility::Static;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_JailBody(TEXT("/Game/VRContent/Modeling/Building_Jail/Jail_Frame.Jail_Frame"));
	if (ActorBaseMesh)
	{
		ActorBaseMesh->Mobility = EComponentMobility::Static;
		ActorBaseMesh->SetupAttachment(StaticBuildingsRoot);
		ActorBaseMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		if (ModelingFinder_JailBody.Succeeded())
		{
			ActorBaseMesh->SetStaticMesh(ModelingFinder_JailBody.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_JailExitDoor(TEXT("/Game/VRContent/Modeling/Building_Jail/ExitDoor.ExitDoor"));
	JailExitDoor = CreateDefaultSubobject<UStaticMeshComponent>("JailExitDoor_SMComp");
	if (JailExitDoor)
	{
		JailExitDoor->Mobility = EComponentMobility::Movable;
		JailExitDoor->SetupAttachment(MoveableBuildingsRoot);
		JailExitDoor->SetRelativeLocation(FVector(0.0f, -245.0f, -0.5f));
		JailExitDoor->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		if (ModelingFinder_JailExitDoor.Succeeded())
		{
			JailExitDoor->SetStaticMesh(ModelingFinder_JailExitDoor.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_JailHatch(TEXT("/Game/VRContent/Modeling/Building_Jail/Roof_Hatch.Roof_Hatch"));
	JailHatch = CreateDefaultSubobject<UStaticMeshComponent>("JailHatch_SMComp");
	if (JailHatch)
	{
		JailHatch->Mobility = EComponentMobility::Movable;
		JailHatch->SetupAttachment(MoveableBuildingsRoot);
		JailHatch->SetRelativeLocation(FVector(0.0f, 0.0f, 3850.0f));
		JailHatch->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		if (ModelingFinder_JailHatch.Succeeded())
		{
			JailHatch->SetStaticMesh(ModelingFinder_JailHatch.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_JailRoof(TEXT("/Game/VRContent/Modeling/Building_Jail/Roof.Roof"));
	JailRoof = CreateDefaultSubobject<UStaticMeshComponent>("JailRoof_SMComp");
	if (JailRoof)
	{
		JailRoof->SetMobility(EComponentMobility::Static);
		JailRoof->SetupAttachment(StaticBuildingsRoot);
		JailRoof->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		if (ModelingFinder_JailRoof.Succeeded())
		{
			JailRoof->SetStaticMesh(ModelingFinder_JailRoof.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_JailWeaponDoor(TEXT("/Game/VRContent/Modeling/Building_Jail/WeaponDoor.WeaponDoor"));
	Jail1FWeaponDoor = CreateDefaultSubobject<UStaticMeshComponent>("Jail1FWeaponDoor_SMComp");
	Jail2FWeaponDoor = CreateDefaultSubobject<UStaticMeshComponent>("Jail2FWeaponDoor_SMComp");
	Jail3FWeaponDoor = CreateDefaultSubobject<UStaticMeshComponent>("Jail3FWeaponDoor_SMComp");
	if (Jail1FWeaponDoor && Jail2FWeaponDoor && Jail3FWeaponDoor)
	{
		FRotator TempCommonRot = FRotator(0.0f, -90.0f, 0.0f);

		Jail1FWeaponDoor->SetMobility(EComponentMobility::Movable);
		Jail1FWeaponDoor->SetupAttachment(FirstFloorRoot);
		Jail1FWeaponDoor->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		Jail1FWeaponDoor->SetRelativeRotation(TempCommonRot);

		Jail2FWeaponDoor->SetMobility(EComponentMobility::Movable);
		Jail2FWeaponDoor->SetupAttachment(SecondFloorRoot);
		Jail2FWeaponDoor->SetRelativeLocation(FVector(0.0f, 0.0f, 929.25f));
		Jail2FWeaponDoor->SetRelativeRotation(TempCommonRot);

		Jail3FWeaponDoor->SetMobility(EComponentMobility::Movable);
		Jail3FWeaponDoor->SetupAttachment(ThirdFloorRoot);
		Jail3FWeaponDoor->SetRelativeLocation(FVector(0.0f, 0.0f, 1860.0f));
		Jail3FWeaponDoor->SetRelativeRotation(TempCommonRot);

		if (ModelingFinder_JailWeaponDoor.Succeeded())
		{
			Jail1FWeaponDoor->SetStaticMesh(ModelingFinder_JailWeaponDoor.Object);
			Jail2FWeaponDoor->SetStaticMesh(ModelingFinder_JailWeaponDoor.Object);
			Jail3FWeaponDoor->SetStaticMesh(ModelingFinder_JailWeaponDoor.Object);
		}
	}

	CLSubdueHatch = CreateDefaultSubobject<UBoxComponent>("CL_TopEscapeSubdue");
	if (CLSubdueHatch)
	{
		CLSubdueHatch->SetupAttachment(JailHatch);
		CLSubdueHatch->SetRelativeLocation(FVector(0.0f, 0.0f, -125.0f));
		CLSubdueHatch->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
		CLSubdueHatch->SetHiddenInGame(false); // Debug
	}

	Jail1FWeaponDoor->SetCanEverAffectNavigation(false);
	Jail2FWeaponDoor->SetCanEverAffectNavigation(false);
	Jail3FWeaponDoor->SetCanEverAffectNavigation(false);
	JailExitDoor->SetCanEverAffectNavigation(false);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Hatch(TEXT("/Game/VRContent/Material/SRS_STAGE_hatch.SRS_STAGE_hatch"));
	if (MaterialFinder_Hatch.Succeeded())
	{
		JailExitDoor->SetMaterial(0, MaterialFinder_Hatch.Object);
		JailHatch->SetMaterial(0, MaterialFinder_Hatch.Object);
		JailRoof->SetMaterial(0, MaterialFinder_Hatch.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Wall(TEXT("/Game/VRContent/Material/SRS_STAGE_WALL.SRS_STAGE_WALL"));
	if (MaterialFinder_Wall.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Wall.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		Jail1FWeaponDoor->SetMaterial(0, MaterialFinder_Main.Object);
		Jail2FWeaponDoor->SetMaterial(0, MaterialFinder_Main.Object);
		Jail3FWeaponDoor->SetMaterial(0, MaterialFinder_Main.Object);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_Glove(TEXT("/Game/VRContent/Blueprints/TimelineCurve/Glove_MoveCurve.Glove_MoveCurve"));
	if (CurveFinder_Glove.Succeeded())
		MoveTheWeaponDoorFloatCurve = CurveFinder_Glove.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_ExitDoor(TEXT("/Game/VRContent/Blueprints/TimelineCurve/ExitDoorSide_Curve.ExitDoorSide_Curve"));
	if (CurveFinder_ExitDoor.Succeeded())
		MoveTheExitDoorFloatCurve = CurveFinder_ExitDoor.Object;

	UpwardMoveTimelineComp = CreateDefaultSubobject<UTimelineComponent>("UpwardMoveTLComp");
	DownwardMoveTimelineComp = CreateDefaultSubobject<UTimelineComponent>("DownwardMoveTLComp");
	SideWardsMoveTimelineComp = CreateDefaultSubobject<UTimelineComponent>("SidewardMoveTLComp");
	UpwardMoveTimelineComp->SetLooping(false);
	DownwardMoveTimelineComp->SetLooping(false);
	SideWardsMoveTimelineComp->SetLooping(false);
	UpwardMoveTimelineComp->SetTimelineLength(1.01f);
	DownwardMoveTimelineComp->SetTimelineLength(1.01f);
	SideWardsMoveTimelineComp->SetTimelineLength(4.01f);

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_GloveNJail(TEXT("/Game/VRContent/Sound/Wavs/Glove/sfx_glove.sfx_glove"));
	if (SoundFinder_GloveNJail.Succeeded())
	{
		GloveNJailDoorOperationSFX = SoundFinder_GloveNJail.Object;
	}
}

void AJailBuilding::BeginPlay()
{
	Super::BeginPlay();
	this->EquipmentRegistrable(this);
	if (EquipmentWorldSubSystem)
	{
		EquipmentWorldSubSystem->FEBMoveOrderSignature.AddDynamic(this, &AJailBuilding::HandleJailReceiveByEB);

		// Glove ¡æ Jail : Bind
		EquipmentWorldSubSystem->FCloseDoorSignature.BindUObject(this, &AJailBuilding::HandleJailReceiveByGlove);

		// AB ¡æ Jail : Bind
		EquipmentWorldSubSystem->FGloveOperationSignature.BindUObject(this, &AJailBuilding::HandleJailReceiveByABButton);
	}
	InitRefDoorNVector();

	if (MoveTheWeaponDoorFloatCurve)
	{
		FOnTimelineFloat UpwardProgressFunc;
		UpwardProgressFunc.BindUFunction(this, FName("UpwardMoveTheDoorPlayEvent"));
		UpwardMoveTimelineComp->AddInterpFloat(MoveTheWeaponDoorFloatCurve, UpwardProgressFunc);

		FOnTimelineEvent UpwardFinishedEvent;
		UpwardFinishedEvent.BindUFunction(this, FName("UpwardMoveTheDoorFinishedEvent"));
		UpwardMoveTimelineComp->SetTimelineFinishedFunc(UpwardFinishedEvent);

		FOnTimelineFloat DownwardProgressFunc;
		DownwardProgressFunc.BindUFunction(this, FName("DownwardMoveTheDoorPlayEvent"));
		DownwardMoveTimelineComp->AddInterpFloat(MoveTheWeaponDoorFloatCurve, DownwardProgressFunc);

		FOnTimelineEvent DownwardFinishedEvent;
		DownwardFinishedEvent.BindUFunction(this, FName("DownwardMoveTheDoorFinishedEvent"));
		DownwardMoveTimelineComp->SetTimelineFinishedFunc(DownwardFinishedEvent);
	}

	if (MoveTheExitDoorFloatCurve)
	{
		FOnTimelineFloat SidewardProgressFunc;
		FOnTimelineEvent SidewardFinishedEvent;

		SidewardProgressFunc.BindUFunction(this, FName("SidewardMoveTheExitDoorPlayEvent"));
		SidewardFinishedEvent.BindUFunction(this, FName("SidewardMoveTheExitDoorFinishedEvent"));

		SideWardsMoveTimelineComp->AddInterpFloat(MoveTheExitDoorFloatCurve, SidewardProgressFunc);
		SideWardsMoveTimelineComp->SetTimelineFinishedFunc(SidewardFinishedEvent);
	}

	// Debug
	/*FTimerHandle TempTimer;
	GetWorldTimerManager().SetTimer(
		TempTimer,
		this,
		&AJailBuilding::HandleExitDoor,
		2.0f,
		false
	);*/

}

void AJailBuilding::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}

void AJailBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJailBuilding::HandleExitDoor()
{
	MoveTheExitDoorSideward();
}

void AJailBuilding::InitRefDoorNVector()
{
	SMWeaponDoorArrs.Add(Jail1FWeaponDoor);
	SMWeaponDoorArrs.Add(Jail2FWeaponDoor);
	SMWeaponDoorArrs.Add(Jail3FWeaponDoor);

	TargetDownVecArrs.Add(Jail1FWeaponDoor->GetRelativeLocation());
	TargetDownVecArrs.Add(Jail2FWeaponDoor->GetRelativeLocation());
	TargetDownVecArrs.Add(Jail3FWeaponDoor->GetRelativeLocation());

	TargetUpVecArrs.Add(Jail1FWeaponDoor->GetRelativeLocation() + FVector(0.f, 0.f, 290.f));
	TargetUpVecArrs.Add(Jail2FWeaponDoor->GetRelativeLocation() + FVector(0.f, 0.f, 290.f));
	TargetUpVecArrs.Add(Jail3FWeaponDoor->GetRelativeLocation() + FVector(0.f, 0.f, 290.f));
}

void AJailBuilding::MoveTheDoorUpward()
{
	UpwardMoveTimelineComp->PlayFromStart();
	mSoundPlayer->PlaySoundEffect(this, GloveNJailDoorOperationSFX, Jail1FWeaponDoor->GetComponentLocation());
}

void AJailBuilding::MoveTheDoorDownward()
{
	DownwardMoveTimelineComp->PlayFromStart();
}

void AJailBuilding::MoveTheExitDoorSideward()
{
	SideWardsMoveTimelineComp->PlayFromStart();
}

void AJailBuilding::HandleJailReceiveByEB(FName InTag, int32 InFloor)
{
	if (InTag == FName("EB"))
	{
		CurrFloorNum = InFloor;
	}
}

void AJailBuilding::UpwardMoveTheDoorPlayEvent(float Value)
{
	int32 ActuallyIndex = CurrFloorNum - 1;
	SMWeaponDoorArrs[ActuallyIndex]->SetRelativeLocation(
		FVector(
			FMath::Lerp(
				TargetDownVecArrs[ActuallyIndex],
				TargetUpVecArrs[ActuallyIndex],
				Value
			)
		)
	);
}

void AJailBuilding::DownwardMoveTheDoorPlayEvent(float Value)
{
	int32 ActuallyIndex = CurrFloorNum - 1;
	SMWeaponDoorArrs[ActuallyIndex]->SetRelativeLocation(
		FVector(
			FMath::Lerp(
				TargetUpVecArrs[ActuallyIndex],
				TargetDownVecArrs[ActuallyIndex],
				Value
			)
		)
	);
}

void AJailBuilding::UpwardMoveTheDoorFinishedEvent()
{
	if (IsValid(EquipmentWorldSubSystem))
	{
		EquipmentWorldSubSystem->NotifyPunchStartBroadCast();
		// Jail ¡æ Glove : BroadCast Function
	}
}

void AJailBuilding::DownwardMoveTheDoorFinishedEvent()
{
	//UE_LOG(LogTemp, Log, TEXT("Finish"));
	EquipmentWorldSubSystem->NotifyEBOperationControlBroadCast(true);
	return;
}

void AJailBuilding::SidewardMoveTheExitDoorPlayEvent(float Value)
{
	JailExitDoor->SetRelativeLocation(FVector(
		0.0f,
		Value * -100.0f,
		0.0f
	));
}

void AJailBuilding::SidewardMoveTheExitDoorFinishedEvent()
{
	// GameOver Logic(In GameMode?) Execute Parts!
	UE_LOG(LogTemp, Log, TEXT("GameOver Logic Execute!"));
	return;
}

void AJailBuilding::HandleJailReceiveByGlove()
{
	MoveTheDoorDownward();
}

void AJailBuilding::HandleJailReceiveByABButton()
{
	EquipmentWorldSubSystem->NotifyEBOperationControlBroadCast(false);
	MoveTheDoorUpward();
}

