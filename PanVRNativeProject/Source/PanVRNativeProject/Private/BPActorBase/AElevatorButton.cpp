// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/AElevatorButton.h"
#include "Core/Component/UGrabComp.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "VRPawn/Hand/VRHand.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"
#include "Core/Debug/FDebugLib.h"

AAElevatorButton::AAElevatorButton()
{
	PrimaryActorTick.bCanEverTick = false;

	EBMainRoot = CreateDefaultSubobject<USceneComponent>("SceneComp");
	if (EBMainRoot)
	{
		this->SetRootComponent(EBMainRoot);
		ActorBaseMesh->SetupAttachment(EBMainRoot);
		GC->SetupAttachment(ActorBaseMesh);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_EBMainBody(TEXT("/Game/VRContent/Modeling/ElevatorButton/EB_MainBody.EB_MainBody"));
	if (ModelingFinder_EBMainBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_EBMainBody.Object);
		ActorBaseMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		ActorBaseMesh->SetCanEverAffectNavigation(false);
	}

	if (GC)
	{
		GC->SetRelativeLocation(FVector(0.0f, 6.0f, 1.5f));
		GC->SetRelativeRotation(FRotator(0.0f, 180.0f, -90.0f));
		GC->EEGrabType = EGrabType::HandToObj;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_EBButton1F(TEXT("/Game/VRContent/Modeling/ElevatorButton/EB_Button_1.EB_Button_1"));
	EBB1F = CreateDefaultSubobject<UStaticMeshComponent>("SM_EB_B_1F");
	if (EBB1F)
	{
		EBB1F->SetupAttachment(ActorBaseMesh);
		if (ModelingFinder_EBButton1F.Succeeded())
		{
			EBB1F->SetStaticMesh(ModelingFinder_EBButton1F.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_EBButton2F(TEXT("/Game/VRContent/Modeling/ElevatorButton/EB_Button_2.EB_Button_2"));
	EBB2F = CreateDefaultSubobject<UStaticMeshComponent>("SM_EB_B_2F");
	if (EBB2F)
	{
		EBB2F->SetupAttachment(ActorBaseMesh);
		if (ModelingFinder_EBButton2F.Succeeded())
		{
			EBB2F->SetStaticMesh(ModelingFinder_EBButton2F.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_EBButton3F(TEXT("/Game/VRContent/Modeling/ElevatorButton/EB_Button_3.EB_Button_3"));
	EBB3F = CreateDefaultSubobject<UStaticMeshComponent>("SM_EB_B_3F");
	if (EBB3F)
	{
		EBB3F->SetupAttachment(ActorBaseMesh);
		if (ModelingFinder_EBButton3F.Succeeded())
		{
			EBB3F->SetStaticMesh(ModelingFinder_EBButton3F.Object);
		}
	}

	CLB1F = CreateDefaultSubobject<UBoxComponent>("CL_EB_B_1F");
	CLB2F = CreateDefaultSubobject<UBoxComponent>("CL_EB_B_2F");
	CLB3F = CreateDefaultSubobject<UBoxComponent>("CL_EB_B_3F");

	if (CLB1F && CLB2F && CLB3F)
	{
		CLB1F->SetupAttachment(EBB1F);
		CLB1F->SetRelativeLocation(FVector(-7.0f, 6.0f, 25.0f));
		CLB1F->SetRelativeRotation(FRotator(0.0f , 0.0f, 15.0f));
		CLB1F->SetBoxExtent(FVector(3.0f, 3.0f, 3.0f));
		CLB1F->SetHiddenInGame(false); // Debug

		CLB2F->SetupAttachment(EBB2F);
		CLB2F->SetRelativeLocation(FVector(0.0f, 7.0f, 28.25f));
		CLB2F->SetRelativeRotation(FRotator(0.0f, 0.0f, 15.0f));
		CLB2F->SetBoxExtent(FVector(3.0f, 3.0f, 3.0f));
		CLB2F->SetHiddenInGame(false); // Debug

		CLB3F->SetupAttachment(EBB3F);
		CLB3F->SetRelativeLocation(FVector(7.0f, 6.0f, 25.0f));
		CLB3F->SetRelativeRotation(FRotator(0.0f, 0.0f, 15.0f));
		CLB3F->SetBoxExtent(FVector(3.0f, 3.0f, 3.0f));
		CLB3F->SetHiddenInGame(false); // Debug
	}

	EBMoveRail = CreateDefaultSubobject<USplineComponent>("SplineComp");
	if (EBMoveRail)
	{
		EBMoveRail->SetupAttachment(EBMainRoot);
		
		EBMoveRail->SetTangentsAtSplinePoint(0, FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), ESplineCoordinateSpace::Local, true);
		EBMoveRail->SetTangentsAtSplinePoint(1, FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), ESplineCoordinateSpace::Local, true);

		EBMoveRail->SetLocationAtSplinePoint(0, FVector(0.0f, 0.0f, 0.0f), ESplineCoordinateSpace::Local, true);
		EBMoveRail->SetLocationAtSplinePoint(1, FVector(0.0f, 0.0f, -50.0f), ESplineCoordinateSpace::Local, true);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_EB(TEXT("/Game/VRContent/Material/SRS_Stage_ElevatorButton.SRS_Stage_ElevatorButton"));

	if (MaterialFinder_Main.Succeeded() && MaterialFinder_EB.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
		EBB1F->SetMaterial(0, MaterialFinder_EB.Object);
		EBB2F->SetMaterial(0, MaterialFinder_EB.Object);
		EBB3F->SetMaterial(0, MaterialFinder_EB.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_Pull(TEXT("/Game/VRContent/Sound/Wavs/ElevatorButton/sfx_elevator_pull.sfx_elevator_pull"));
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_ButtonPress(TEXT("/Game/VRContent/Sound/Wavs/ElevatorButton/sfx_elevator_button.sfx_elevator_button"));
	if (SoundFinder_Pull.Succeeded() && SoundFinder_ButtonPress.Succeeded())
	{
		PullSFX = SoundFinder_Pull.Object;
		ButtonPressSFX = SoundFinder_ButtonPress.Object;
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

void AAElevatorButton::BeginPlay()
{
	Super::BeginPlay();

	CLB1F->OnComponentBeginOverlap.AddDynamic(this, &AAElevatorButton::EBBOverlapBegin);
	CLB2F->OnComponentBeginOverlap.AddDynamic(this, &AAElevatorButton::EBBOverlapBegin);
	CLB3F->OnComponentBeginOverlap.AddDynamic(this, &AAElevatorButton::EBBOverlapBegin);

	CLB1F->OnComponentEndOverlap.AddDynamic(this, &AAElevatorButton::EBBFOverlapEnd);
	CLB2F->OnComponentEndOverlap.AddDynamic(this, &AAElevatorButton::EBBFOverlapEnd);
	CLB3F->OnComponentEndOverlap.AddDynamic(this, &AAElevatorButton::EBBFOverlapEnd);

	this->EquipmentRegistrable(this);

	if (EquipmentWorldSubSystem)
	{
		// Jail ¡æ EB : Bind 
		EquipmentWorldSubSystem->FEBOperateControlSignature.BindUObject(this, &AAElevatorButton::HandleEBReceiveByJail);
	}
}

void AAElevatorButton::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}

void AAElevatorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAElevatorButton::EBBOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("HandIndex")))
	{
		if (bIsOverlapping == 0)
		{
			bIsOverlapping = 1;

			mSoundPlayer->PlaySoundEffect(this, ButtonPressSFX, ActorBaseMesh->GetComponentLocation());

			if (OverlappedComp == CLB1F) // 1st Button Overlap Begin!
			{
				SetPressedFloorNum(1);
			}
			else if (OverlappedComp == CLB2F) // 2nd Button Overlap Begin!
			{
				SetPressedFloorNum(2);
			}
			else if (OverlappedComp == CLB3F) // 3rd Button Overlap Begin!
			{
				SetPressedFloorNum(3);
			}

			if (GetCurrFloorNum() != GetPressedFloorNum())
			{
				if (IsValid(EquipmentWorldSubSystem))
				{
					EquipmentWorldSubSystem->NotifyMoveOrderBroadCast(FName("EB"), this->GetPressedFloorNum());
					SetCurrFloorNum(PressedFloorNum);
				}
				else
				{
					return;
				}
			}
			else
			{
				return;
			}
		}
	}
}

void AAElevatorButton::EBBFOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName("HandIndex")))
	{
		if (bIsOverlapping == 1 && !GetWorldTimerManager().IsTimerActive(OverlapDelyTimer))
		{
			if (OverlappedComp == CLB1F) // 1st Button Overlap End!
			{
				GetWorldTimerManager().SetTimer(
					OverlapDelyTimer,
					this,
					&AAElevatorButton::OverlapIndexDelay,
					0.5f,
					false
				);
			}

			if (OverlappedComp == CLB2F) // 2nd Button Overlap End!
			{
				GetWorldTimerManager().SetTimer(
					OverlapDelyTimer,
					this,
					&AAElevatorButton::OverlapIndexDelay,
					0.5f,
					false
				);
			}

			if (OverlappedComp == CLB3F) // 3rd Button Overlap End!
			{
				GetWorldTimerManager().SetTimer(
					OverlapDelyTimer,
					this,
					&AAElevatorButton::OverlapIndexDelay,
					0.5f,
					false
				);
			}
		}
	}
}

void AAElevatorButton::OverlapIndexDelay()
{
	bIsOverlapping = 0;
	if (GetWorldTimerManager().IsTimerActive(OverlapDelyTimer))
	{
		GetWorldTimerManager().PauseTimer(OverlapDelyTimer);
		GetWorldTimerManager().ClearTimer(OverlapDelyTimer);
	}
}

void AAElevatorButton::UpdatePullingMove()
{
	float TempPosZ = (ActorBaseMesh->GetRelativeLocation().Z) + (TempMCRef->GetComponentTransform().GetRelativeTransform(ActorBaseMesh->GetComponentTransform())).GetLocation().Z;

	float TempMin = EBMoveRail->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local).Z;
	float TempMax = EBMoveRail->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local).Z;

	ActorBaseMesh->SetRelativeLocation(
		FVector(
			ActorBaseMesh->GetRelativeLocation().X,
			ActorBaseMesh->GetRelativeLocation().Y,
			FMath::Clamp(TempPosZ, TempMin, TempMax)
		)
	);
}

void AAElevatorButton::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{
	GC->SetRelativeLocation(FVector(0.f, 9.f, -4.f));
	TempMCRef = &InMCRef;
	TempHand = InGrabbingHand;

	if (TempMCRef->ComponentHasTag(FName("RightMC")))
	{
		InGrabbingHand->GetCLCapsuleIndex()->SetGenerateOverlapEvents(false);
	}
	else
	{
		InGrabbingHand->GetCLCapsuleIndex()->SetGenerateOverlapEvents(false);
	}
	mSoundPlayer->PlaySoundEffect(this, PullSFX, ActorBaseMesh->GetComponentLocation());

	GetWorldTimerManager().SetTimer(
		EBMoveTimer,
		this,
		&AAElevatorButton::UpdatePullingMove,
		0.01f,
		true
	);
}

void AAElevatorButton::OnDropped()
{
	TempHand->GetCLCapsuleIndex()->SetGenerateOverlapEvents(true);

	TempMCRef = nullptr;
	TempHand = nullptr;

	if (GetWorldTimerManager().IsTimerActive(EBMoveTimer))
	{
		GetWorldTimerManager().PauseTimer(EBMoveTimer);
		GetWorldTimerManager().ClearTimer(EBMoveTimer);
	}

	UpdatePullingBackMoving();
}

void AAElevatorButton::UpdatePullingBackMoving()
{
	ActorBaseMesh->SetCollisionProfileName(FName("NoCollision"));

	FLatentActionInfo EBLatentInfo;
	EBLatentInfo.CallbackTarget = this;
	EBLatentInfo.ExecutionFunction = FName("UpdatePullingBackMoveCompleted");
	EBLatentInfo.Linkage = 0;
	EBLatentInfo.UUID = 1001;

	UKismetSystemLibrary::MoveComponentTo(
		ActorBaseMesh,
		FVector(0.0f, 0.0f, 0.0f),
		FRotator(0.0f, 90.0f, 0.0f),
		true,
		true,
		1.0f,
		false,
		EMoveComponentAction::Move,
		EBLatentInfo
	);
}

void AAElevatorButton::SetEBColEnabled(uint8 InOperateFlag)
{
	if (InOperateFlag) // Collision Operate Possible
	{
		CLB1F->SetGenerateOverlapEvents(true);
		CLB2F->SetGenerateOverlapEvents(true);
		CLB3F->SetGenerateOverlapEvents(true);
	}
	else // Collision Operate ImPossible
	{
		CLB1F->SetGenerateOverlapEvents(false);
		CLB2F->SetGenerateOverlapEvents(false);
		CLB3F->SetGenerateOverlapEvents(false);
	}
}

void AAElevatorButton::HandleEBReceiveByJail(uint8 InEBControlFlag)
{
	SetEBColEnabled(InEBControlFlag);
}



void AAElevatorButton::UpdatePullingBackMoveCompleted()
{
	ActorBaseMesh->SetCollisionProfileName(FName("PhysicsActor"));
}

int32 AAElevatorButton::GetCurrFloorNum() const
{
	return CurrFloor;
}
int32 AAElevatorButton::GetPressedFloorNum() const
{
	return PressedFloorNum;
}
void AAElevatorButton::SetCurrFloorNum(int32 InCurrFloor)
{
	CurrFloor = InCurrFloor;
	return;
}
void AAElevatorButton::SetPressedFloorNum(int32 InPressFloor)
{
	PressedFloorNum = InPressFloor;
	return;
}
