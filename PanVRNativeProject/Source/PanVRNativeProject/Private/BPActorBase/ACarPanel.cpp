// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/ACarPanel.h"
#include "Core/Component/UGrabComp.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "BPActorBase/ACarKey.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"

AACarPanel::AACarPanel()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create CarPanel Main Root And Setting Root And Hierarchy
	CarMainRoot = CreateDefaultSubobject<USceneComponent>("SC_Main");
	if (CarMainRoot)
	{
		// Setting Components Hierarchy
		SetRootComponent(CarMainRoot);
		ActorBaseMesh->SetupAttachment(CarMainRoot);
		GC->SetupAttachment(ActorBaseMesh);
	}

	// ActorBaseMesh Setting StaticMesh And Pos,Rot
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_MainHandle(TEXT("/Game/VRContent/Modeling/CarPanel/SM_Car_MainHandle.SM_Car_MainHandle"));
	if (ModelingFinder_MainHandle.Succeeded())
	{
		// Actually Setting Static Mesh
		ActorBaseMesh->SetStaticMesh(ModelingFinder_MainHandle.Object);
		// ActorBaseMesh Setting Pos N Rot
		ActorBaseMesh->SetRelativeLocation(FVector(0.0f, -1.5f, 1.0f));
		ActorBaseMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 37.5f));
	}

	// MainHandle Rotate Standard Create And Setting
	MainHandleStandardRoot = CreateDefaultSubobject<USceneComponent>("SC_MaindHandle");
	if (MainHandleStandardRoot)
	{
		// Setting Component Hierarchy
		MainHandleStandardRoot->SetupAttachment(ActorBaseMesh);
		MainHandleStandardRoot->SetRelativeLocation(FVector(0.0f, 0.0f, 27.0f));
	}

	// Main Handle GC Setting
	if (GC)
	{
		GC->SetRelativeLocation(FVector(0.0f, 0.0f, 27.0f));
		GC->EEGrabType = EGrabType::HandToObj;
	}

	// Main Body Create And Modeling Find
	CarMainBody = CreateDefaultSubobject<UStaticMeshComponent>("SM_MainBody");
	if (CarMainBody)
	{
		CarMainBody->SetupAttachment(CarMainRoot);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_MainBody(TEXT("/Game/VRContent/Modeling/CarPanel/SM_Car_Main_Body.SM_Car_Main_Body"));
	if (ModelingFinder_MainBody.Succeeded())
	{
		CarMainBody->SetStaticMesh(ModelingFinder_MainBody.Object);
	}

	// Setting Sub Handle

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_SubHandle(TEXT("/Game/VRContent/Modeling/CarPanel/SM_Car_SubHandle.SM_Car_SubHandle"));

	CarSubHandle = CreateDefaultSubobject<UStaticMeshComponent>("SM_SubHandle");
	if (CarSubHandle)
	{
		CarSubHandle->SetupAttachment(CarMainBody);
		CarSubHandle->SetRelativeLocation(FVector(20.0f, -7.5f, 23.8f));
		CarSubHandle->SetRelativeRotation(FRotator(0.0f, 0.0f, 30.0f));
		if (ModelingFinder_SubHandle.Succeeded())
		{
			CarSubHandle->SetStaticMesh(ModelingFinder_SubHandle.Object);
		}
	}
	SubHandleGC = CreateDefaultSubobject<UUGrabComp>("GC_SubHandle");
	if (SubHandleGC)
	{
		SubHandleGC->SetupAttachment(CarSubHandle);
		SubHandleGC->SetRelativeLocation(FVector(1.75f, 3.0f, 14.5f));
		SubHandleGC->EEGrabType = EGrabType::HandToObj;
	}
	SubHandleStandardRoot = CreateDefaultSubobject<USceneComponent>("SC_SubHandle");
	if (SubHandleStandardRoot)
	{
		SubHandleStandardRoot->SetupAttachment(CarSubHandle);
		SubHandleStandardRoot->SetRelativeLocation(FVector(1.75f, 0.0f, 14.5f));
	}
	CLSubHandle = CreateDefaultSubobject<UCapsuleComponent>("CL_SubHandle");
	if (CLSubHandle)
	{
		CLSubHandle->SetupAttachment(CarSubHandle);
		CLSubHandle->SetRelativeLocation(FVector(1.75f, 0.0f, 14.5f));
		CLSubHandle->SetCapsuleRadius(1.7f);
		CLSubHandle->SetCapsuleHalfHeight(5.5f);
		CLSubHandle->SetVisibility(true); // Debug
		CLSubHandle->SetHiddenInGame(false); // Debug
	}

	// Setting LidDoor
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_LidDoor(TEXT("/Game/VRContent/Modeling/CarPanel/SM_Car_LidDoor.SM_Car_LidDoor"));

	CarLidDoor = CreateDefaultSubobject<UStaticMeshComponent>("SM_LidDoor");
	if (CarLidDoor)
	{
		CarLidDoor->SetupAttachment(CarMainBody);
		CarLidDoor->SetRelativeLocation(FVector(15.5f, 2.3f, 7.3f));
		if (ModelingFinder_LidDoor.Succeeded())
		{
			CarLidDoor->SetStaticMesh(ModelingFinder_LidDoor.Object);
		}
	}
	LidDoorGC = CreateDefaultSubobject<UUGrabComp>("GC_LidDoor");
	if (LidDoorGC)
	{
		LidDoorGC->SetupAttachment(CarLidDoor);
		LidDoorGC->SetRelativeLocation(FVector(0.0f, 3.9f, -6.3f));
		LidDoorGC->EEGrabType = EGrabType::AutomaticObj;
	}

	// Setting CarKey Spawn Part
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_CarKey(TEXT("/Game/VRContent/Modeling/CarPanel/SM_CarKey.SM_CarKey"));

	CarKey = CreateDefaultSubobject<UStaticMeshComponent>("SM_CarKey");
	if (CarKey)
	{
		CarKey->SetupAttachment(CarMainBody);
		CarKey->SetRelativeLocation(FVector(13.6f, 1.8f, -8.4f));
		CarKey->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		if (ModelingFinder_CarKey.Succeeded())
		{
			CarKey->SetStaticMesh(ModelingFinder_CarKey.Object);
		}
	}
	CLCarKey = CreateDefaultSubobject<UBoxComponent>("CL_CarKey");
	if (CLCarKey)
	{
		CLCarKey->SetupAttachment(CarKey);
		CLCarKey->SetRelativeLocation(FVector(3.5f, 0.0f, -2.0f));
		CLCarKey->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		CLCarKey->SetBoxExtent(FVector(7.0f, 4.0f, 3.0f));
		CLCarKey->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CLCarKey->SetVisibility(true); // Debug
		CLCarKey->SetHiddenInGame(false); // Debug
	}
	CarKeyGC = CreateDefaultSubobject<UUGrabComp>("GC_CarKey");
	if (CarKeyGC)
	{
		CarKeyGC->SetupAttachment(CarKey);
		CarKeyGC->SetRelativeLocation(FVector(3.5f, 0.0f, -2.0f));
		CarKeyGC->SetRelativeRotation(FRotator(90.0f, 15.4f, 15.4f));
		CarKeyGC->EEGrabType = EGrabType::SpawnObj;
	}

	// Setting KeyHole And Body
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_KeyHole(TEXT("/Game/VRContent/Modeling/CarPanel/SM_Car_KeyHole.SM_Car_KeyHole"));

	KeyHole = CreateDefaultSubobject<UStaticMeshComponent>("SM_KeyHole");
	if (KeyHole)
	{
		KeyHole->SetupAttachment(CarMainBody);
		KeyHole->SetRelativeLocation(FVector(15.0f, -18.0f, 32.5f));
		KeyHole->SetRelativeRotation(FRotator(0.0f, 0.0f, 28.0f));
		if (ModelingFinder_KeyHole.Object)
		{
			KeyHole->SetStaticMesh(ModelingFinder_KeyHole.Object);
		}
	}
	CLKeyHole = CreateDefaultSubobject<UBoxComponent>("CL_KeyHole");
	if (CLKeyHole)
	{
		CLKeyHole->SetupAttachment(KeyHole);
		CLKeyHole->SetRelativeLocation(FVector(0.0f, 0.0f, 1.0f));
		CLKeyHole->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
		CLKeyHole->SetBoxExtent(FVector(5.0f, 0.5f, 4.0f));
		CLKeyHole->SetVisibility(true); // Debug
		CLKeyHole->SetHiddenInGame(false); // Debug
	}

	// Setting KeyHole Body
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_KeyHoleBody(TEXT("/Game/VRContent/Modeling/CarPanel/SM_Car_KeyHoleBody.SM_Car_KeyHoleBody"));

	KeyHoleBody = CreateDefaultSubobject<UStaticMeshComponent>("SM_KeyHoleBody");
	if (KeyHoleBody)
	{
		KeyHoleBody->SetupAttachment(CarMainBody);
		KeyHoleBody->SetRelativeLocation(FVector(15.0f, -18.0f, 32.5f));
		KeyHoleBody->SetRelativeRotation(FRotator(0.0f, 0.0f, 28.0f));
		if (ModelingFinder_KeyHoleBody.Succeeded())
		{
			KeyHoleBody->SetStaticMesh(ModelingFinder_KeyHoleBody.Object);
		}
	}

	// Setting Glasses
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_GlassOne(TEXT("/Game/VRContent/Modeling/CarPanel/SM_Car_Glass1.SM_Car_Glass1"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_GlassTwo(TEXT("/Game/VRContent/Modeling/CarPanel/SM_Car_Glass2.SM_Car_Glass2"));

	CarPanelGlass1 = CreateDefaultSubobject<UStaticMeshComponent>("SM_Glass1");
	CarPanelGlass2 = CreateDefaultSubobject<UStaticMeshComponent>("SM_Glass2");
	if (CarPanelGlass1 && CarPanelGlass2)
	{
		CarPanelGlass1->SetupAttachment(CarMainBody);
		CarPanelGlass2->SetupAttachment(CarMainBody);
		if (ModelingFinder_GlassOne.Succeeded() && ModelingFinder_GlassTwo.Succeeded())
		{
			CarPanelGlass1->SetStaticMesh(ModelingFinder_GlassOne.Object);
			CarPanelGlass2->SetStaticMesh(ModelingFinder_GlassTwo.Object);
		}
	}

	// 
	AtFirstMainHandleGCVec = GC->GetRelativeLocation();
	AtFirstSubHandleGCVec = SubHandleGC->GetRelativeLocation();
	AfFirstSubHandleRot = CarSubHandle->GetRelativeRotation();

	// Bind KeyHole Collision Overlap Begin And End Function
	CLKeyHole->OnComponentBeginOverlap.AddDynamic(this, &AACarPanel::OverlapBeginByCarKey);
	CLKeyHole->OnComponentEndOverlap.AddDynamic(this, &AACarPanel::OverlapEndByCarkey);

	// Create TimelineComponent And Constructed Timeline Related Data
	CarPanelTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));
	CarPanelTimeline->SetLooping(false);
	CarPanelTimeline->SetTimelineLength(1.01f);

	// 
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_KeyHoleCurve(TEXT("/Game/VRContent/Blueprints/TimelineCurve/CarPanel_KeyHole_MoveCurve.CarPanel_KeyHole_MoveCurve"));
	
	if (CurveFinder_KeyHoleCurve.Succeeded())
	{
		KeyHoleMoveCurve = CurveFinder_KeyHoleCurve.Object;
	}

	// 
	ShakeRot.Roll = CarMainBody->GetRelativeRotation().Roll;		//CarMainBody->GetComponentRotation().Roll;
	ShakeRot.Pitch = CarMainBody->GetRelativeRotation().Pitch;	 //CarMainBody->GetComponentRotation().Pitch;
	ShakeRot.Yaw = CarMainBody->GetRelativeRotation().Yaw;	 //CarMainBody->GetComponentRotation().Yaw;

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
		CarMainBody->SetMaterial(0, MaterialFinder_Main.Object);
		CarSubHandle->SetMaterial(0, MaterialFinder_Main.Object);
		CarLidDoor->SetMaterial(0, MaterialFinder_Main.Object);
		CarKey->SetMaterial(0, MaterialFinder_Main.Object);
		KeyHole->SetMaterial(0, MaterialFinder_Main.Object);
		KeyHoleBody->SetMaterial(0, MaterialFinder_Main.Object);
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

void AACarPanel::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat Progress;
	Progress.BindUFunction(this, FName("KeyHoleOperatingTimeline"));

	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindUFunction(this, FName("KeyHoleOperatingTimelineEnd"));

	CarPanelTimeline->AddInterpFloat(KeyHoleMoveCurve, Progress);
	CarPanelTimeline->SetTimelineFinishedFunc(FinishedEvent);
}

void AACarPanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CarPanelTimeline)
	{
		CarPanelTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}

	if (MainBodyShakingFlag)
	{
		float TempShakingDegree = FMath::RandRange(ShakingLevel * -1.0f, ShakingLevel);
		FRotator FinalShakeRot = FRotator(ShakeRot.Pitch + TempShakingDegree, ShakeRot.Yaw + TempShakingDegree, ShakeRot.Roll + TempShakingDegree);

		CarMainBody->SetRelativeRotation(FinalShakeRot);
	}
}

void AACarPanel::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{
	if (GC->MCRef == &InMCRef)
	{
		if (GC->EEGrabType == EGrabType::HandToObj)
		{
			if (InMCRef.ComponentHasTag(FName("RightMC")))
			{
				FVector MHGCRightGrabbedPos = AtFirstMainHandleGCVec + FVector(16.5f, 4.0f, 10.0f);
				GC->SetRelativeLocation(MHGCRightGrabbedPos);
				GC->SetRelativeRotation(FRotator(0.0f, 285.0f, 90.0f));
			}
			else
			{
				FVector MHGCLeftGrabbedPos = AtFirstMainHandleGCVec + FVector(-16.5f, 4.0f, 10.0f);
				GC->SetRelativeLocation(MHGCLeftGrabbedPos);
				GC->SetRelativeRotation(FRotator(0.0f, 75.0f, 90.0f));
			}

			if (!bIsCoolDown)
			{
				UE_LOG(LogTemp, Log, TEXT("Main Handle Operation Part!"));
				PrevVec = ActorBaseMesh->GetRightVector().GetSafeNormal(0.0001f);

				GetWorldTimerManager().SetTimer(
					MainHandleOperateTimer,
					this,
					&AACarPanel::MainHandleOperating,
					0.01f,
					true
				);
			}
			else
			{
				GetWorldTimerManager().PauseTimer(MainHandleOperateTimer);
			}

		}
	}
	else if (SubHandleGC->MCRef == &InMCRef)
	{
		if (SubHandleGC->EEGrabType == EGrabType::HandToObj)
		{
			if (InMCRef.ComponentHasTag(FName("RightMC")))
			{
				FVector SHGCRightGrabbedPos = AtFirstSubHandleGCVec + FVector(2.0f, 5.0f, 0.0f);
				SubHandleGC->SetRelativeLocation(SHGCRightGrabbedPos);
				SubHandleGC->SetRelativeRotation(FRotator(90.0f, 0.0f, 180.0f));
			}
			else
			{
				FVector SHGCLeftGrabbedPos = AtFirstSubHandleGCVec + FVector(-2.0f, 5.0f, 0.0f);
				SubHandleGC->SetRelativeLocation(SHGCLeftGrabbedPos);
				SubHandleGC->SetRelativeRotation(FRotator(-90.0f, 0.0f, 180.0f));
			}
			UE_LOG(LogTemp, Log, TEXT("Sub Handle Operationg Part Execution!"));
			GetWorldTimerManager().SetTimer(
				SubHandleOperateTimer,
				this,
				&AACarPanel::SubHandleOperating,
				0.01f,
				true
			);
		}
	}
	else if (LidDoorGC->MCRef == &InMCRef)
	{
		if (LidDoorGC->EEGrabType == EGrabType::AutomaticObj)
		{
			if (CarLidDoor->GetRelativeRotation().Roll == 0)
			{
				CarLidDoor->SetRelativeRotation(FRotator(0.0f, 0.0f, -110.0f));
				this->OnDropped();
				CLCarKey->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
			else
			{
				CarLidDoor->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
				this->OnDropped();
				CLCarKey->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
	else if (CarKeyGC->MCRef == &InMCRef)
	{
		UWorld* TempMyWorld = GetWorld(); // Current World
		FTransform TempSpawnTransform(FTransform::Identity); // Default Spanw Trnasform
		if (CarKeyGC->EEGrabType == EGrabType::SpawnObj)
		{
			if (CLCarKey->GetCollisionEnabled() == ECollisionEnabled::QueryAndPhysics)
			{
				NewSpawnCarKey = TempMyWorld->SpawnActor<AACarKey>(AACarKey::StaticClass(), TempSpawnTransform);
				NewSpawnCarKey->OnGrabbed(InMCRef, HandGrabPos, InGrabbingHand);
				NewSpawnCarKey->AttachToComponent(&InMCRef, FAttachmentTransformRules::SnapToTargetIncludingScale);

				if (InMCRef.ComponentHasTag(FName("RightMC")))
				{
					FVector SpawnCarKeyVecInHand = (NewSpawnCarKey->GetRootComponent()->GetRelativeLocation() + FVector(0.0f, -2.0f, 0.0f));
					NewSpawnCarKey->SetActorRelativeLocation(SpawnCarKeyVecInHand);
					NewSpawnCarKey->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
				}
				else
				{
					FVector SpawnCarKeyVecInHand = (NewSpawnCarKey->GetRootComponent()->GetRelativeLocation() + FVector(0.0f, 2.0f, 0.0f));
					NewSpawnCarKey->SetActorRelativeLocation(SpawnCarKeyVecInHand);
					NewSpawnCarKey->SetActorRelativeRotation(FRotator(180.0f, 0.0f, -90.0f));
				}
			}
			else
			{
				this->OnDropped();
				return;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("GC Wrong Operating!"));
		this->OnDropped();
		return;
	}
	return;
}

void AACarPanel::OnDropped()
{
	if (GC->MCRef)
	{
		GetWorldTimerManager().PauseTimer(MainHandleOperateTimer);
		GC->SetRelativeLocation(AtFirstMainHandleGCVec);
		GC->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		GC->MCRef = nullptr;
	}
	else if (SubHandleGC->MCRef)
	{
		GetWorldTimerManager().ClearTimer(SubHandleOperateTimer);
		CarSubHandle->SetRelativeRotation(AfFirstSubHandleRot);
		SubHandleGC->SetRelativeLocation(AtFirstSubHandleGCVec);
		SubHandleGC->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	else if (LidDoorGC->MCRef) {}
	else if (CarKeyGC->MCRef) {}
}

void AACarPanel::SubHandleOperating()
{
	// Between Sub Handle Standard And Sub Handle GC In Motion Controller World Transform
	FTransform BetWeen_SHStandardAndSHMC = SubHandleGC->MCRef->GetComponentTransform().GetRelativeTransform(SubHandleStandardRoot->GetComponentTransform());

	float TempRotation = FMath::Clamp(CarSubHandle->GetRelativeRotation().Pitch - BetWeen_SHStandardAndSHMC.GetLocation().X, -90.0f, 0.0f);

	FQuat TempQuatOne = FRotator(TempRotation, 0.0f, 0.0f).Quaternion();
	FQuat TempQuatTwo = FRotator(0.0f, 0.0f, 30.0f).Quaternion();

	CarSubHandle->SetRelativeRotation((TempQuatTwo * TempQuatOne).Rotator());
}

void AACarPanel::MainHandleOperating()
{
	FRotator TempRot = UKismetMathLibrary::FindLookAtRotation(ActorBaseMesh->GetComponentLocation(), GC->MCRef->GetComponentLocation());
	double Tempdouble = ActorBaseMesh->GetRightVector().Dot(TempRot.RotateVector(FVector(1.0f, 0.0f, 0.0f)));
	ActorBaseMesh->AddLocalRotation(FRotator(0.0f, UKismetMathLibrary::NormalizeAxis(Tempdouble) * 15.0f, 0.0f));
}

void AACarPanel::OverlapBeginByCarKey(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsCarKeyAttaching)
	{
		if (OtherComp->ComponentHasTag(FName("CarKey")))
		{
			bIsTurnOnCar = true;
			//bIsTurnOnCar = FMath::RandBool();

			AACarKey* ActuallyOverlapCarKey = Cast<AACarKey>(OtherActor);
			if (ActuallyOverlapCarKey)
			{
				AttachedCarKey = ActuallyOverlapCarKey;
			}

			if (ActuallyOverlapCarKey != NewSpawnCarKey)
			{
				return;
			}

			if (bIsTurnOnCar)
			{
				if (AttachedCarKey)
				{
					bIsCarKeyAttaching = 1;
					OtherActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					OtherActor->AttachToComponent(KeyHole, FAttachmentTransformRules::KeepWorldTransform);
					AttachedCarKey->GC->SetPrimitiveCompPhysics(false);
					NewSpawnCarKey = nullptr;
					AttachedCarKey->GC->CurrentIGrabInterface = nullptr;
					AttachedCarKey->SetActorRelativeLocation(FVector(0.0f, 0.0f, 3.85f));
					AttachedCarKey->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
					AttachedCarKey->SetDontGrabPhysics();
					CarPanelTimeline->PlayFromStart();
					bIsCoolDown = 1;
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Car Turn On Fail!"));
			}
		}
		else
		{
			return;
		}
	}
}

void AACarPanel::OverlapEndByCarkey(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}

// 
void AACarPanel::KeyHoleOperatingTimeline(float value)
{
	FQuat TempQuatOne = FRotator(0.0f, 0.0f, 28.0f).Quaternion();
	FQuat TempQuatTwo = FRotator(0.0f, value * 10.0f, 0.0f).Quaternion();
	FRotator FinalRot = (TempQuatOne * TempQuatTwo).Rotator();

	KeyHole->SetRelativeRotation(FinalRot);
}

void AACarPanel::KeyHoleOperatingTimelineEnd()
{
	MainBodyShakingFlag = true;
	GetWorldTimerManager().SetTimer(
		DelayTimer,
		this,
		&AACarPanel::KeyHoleOperatingEnd,
		6.0f,
		false
	);
}

void AACarPanel::KeyHoleOperatingEnd()
{
	MainBodyShakingFlag = false;
	CarMainBody->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	KeyHole->SetRelativeRotation(FRotator(0.0f, 0.0f, 28.0f));
	if (IsValid(AttachedCarKey))
		AttachedCarKey->Destroy();
	bIsCarKeyAttaching = 0;
	AttachedCarKey = nullptr;
	bIsCoolDown = 0;
}