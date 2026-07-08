// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn/CVRPawn.h"
#include "VRPawn/Hand/VRHand.h"
#include "Camera/CameraComponent.h"
#include "IXRTrackingSystem.h"
#include "IHeadMountedDisplay.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreObj/VRGameInstance.h"
#include "CoreObj/VRGameInstanceSubsystem.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"


ACVRPawn::ACVRPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	ChairPlatform = CreateDefaultSubobject<UStaticMeshComponent>("SMComp_ChairPlatform");
	ChairBody = CreateDefaultSubobject<UStaticMeshComponent>("SMComp_Body");
	ChairTowerHead = CreateDefaultSubobject<UStaticMeshComponent>("SMComp_TowerHead");

	if (ChairPlatform && ChairBody && ChairTowerHead)
	{
		ChairPlatform->SetupAttachment(Root);
		ChairBody->SetupAttachment(Root);
		ChairTowerHead->SetupAttachment(ChairBody);

		ChairPlatform->SetRelativeLocation(FVector(0.f, 0.f, -94.8f));
		ChairPlatform->SetRelativeRotation(FRotator(0.f, -90.0f, 0.f));

		ChairBody->SetRelativeLocation(FVector(0.f, 0.f, -86.3f));
		ChairBody->SetRelativeRotation(FRotator(0.f, -90.0f, 0.f));

		ChairTowerHead->SetRelativeLocation(FVector(0.f, 0.f, 45.0f));
		ChairTowerHead->SetRelativeRotation(FRotator(0.f, 180.0f, 0.f));
	}

	VRPawnMovementTimeline = CreateDefaultSubobject<UTimelineComponent>("VRPawn_TimelineComp");
	if (VRPawnMovementTimeline)
	{
		VRPawnMovementTimeline->SetLooping(false);
		VRPawnMovementTimeline->SetTimelineLength(2.01f);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_MoveUp(TEXT("/Game/VRContent/Blueprints/TimelineCurve/PlayerUpDown_Move_Curve.PlayerUpDown_Move_Curve"));
	if (CurveFinder_MoveUp.Succeeded())
	{
		VRPawnMoveUpCurve = CurveFinder_MoveUp.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ChairPlatform(TEXT("/Game/VRContent/Modeling/Stool(Chair)/ChairFlatform.ChairFlatform"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Lobby(TEXT("/Game/VRContent/Material/SRS_LOBBY.SRS_LOBBY"));
	if (ModelingFinder_ChairPlatform.Succeeded() && MaterialFinder_Lobby.Succeeded())
	{
		ChairPlatform->SetStaticMesh(ModelingFinder_ChairPlatform.Object);
		ChairPlatform->SetMaterial(0, MaterialFinder_Lobby.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ChairBody(TEXT("/Game/VRContent/Modeling/Stool(Chair)/StoolChair.StoolChair"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (ModelingFinder_ChairBody.Succeeded() && MaterialFinder_Main.Succeeded())
	{
		ChairBody->SetStaticMesh(ModelingFinder_ChairBody.Object);
		ChairBody->SetMaterial(0, MaterialFinder_Main.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_TowerHead(TEXT("/Game/VRContent/Modeling/Stool(Chair)/TowerHead.TowerHead"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Tower(TEXT("/Game/VRContent/Material/SRS_STAGE_TOWER.SRS_STAGE_TOWER"));
	if (ModelingFinder_TowerHead.Succeeded() && MaterialFinder_Tower.Succeeded())
	{
		ChairTowerHead->SetStaticMesh(ModelingFinder_TowerHead.Object);
		ChairTowerHead->SetMaterial(0, MaterialFinder_Tower.Object);
	}

	static ConstructorHelpers::FClassFinder<AVRHand> ClassFinder_LeftHand(TEXT("/Game/VRContent/Blueprints/BP_VRLeftHand.BP_VRLeftHand_C"));
	static ConstructorHelpers::FClassFinder<AVRHand> ClassFinder_RightHand(TEXT("/Game/VRContent/Blueprints/BP_VRRightHand.BP_VRRightHand_C"));
	if (ClassFinder_LeftHand.Succeeded() && ClassFinder_RightHand.Succeeded())
	{
		LeftHandBPClass = ClassFinder_LeftHand.Class;
		RightHandBPClass = ClassFinder_RightHand.Class;
	}

	if (GetMesh())
	{
		GetMesh()->SetEnableGravity(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetEnableGravity(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> ContextFinder_Default(TEXT("/Game/VRTemplate/Input/IMC_Default.IMC_Default"));
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> ContextFinder_Hands(TEXT("/Game/VRTemplate/Input/IMC_Hands.IMC_Hands"));
	if (ContextFinder_Default.Succeeded() && ContextFinder_Hands.Succeeded())
	{
		IMC_Default = ContextFinder_Default.Object;
		IMC_Hands = ContextFinder_Hands.Object;
	}

	this->GetCharacterMovement()->GravityScale = 0.0f;
	this->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	InitFloorData();
}

void ACVRPawn::BeginPlay()
{
	Super::BeginPlay();

	this->SpawnHands();

	if (GEngine && GEngine->XRSystem.IsValid())
	{
		IHeadMountedDisplay* HMDDevice = GEngine->XRSystem->GetHMDDevice();
		if (HMDDevice)
		{
			// Is Head Mounted Display Enabled Value True;
			if (HMDDevice->IsHMDEnabled())
			{
				UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
				GEngine->Exec(GetWorld(), TEXT("vr.PixelDensity 1.0"));

				if (APlayerController* myPC = Cast<APlayerController>(GetController()))
				{
					if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(myPC->GetLocalPlayer()))
					{
						if (IMC_Default)
							Subsystem->AddMappingContext(IMC_Default, 0);
						if (IMC_Hands)
							Subsystem->AddMappingContext(IMC_Hands, 0);
					}
				}
			}
		}
	}

	FOnTimelineFloat ChangeValue;
	ChangeValue.BindUFunction(this, FName("VRPawnMoveUpTLFunc"));
	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindUFunction(this, FName("VRPawnMoveUpTLEndFunc"));

	VRPawnMovementTimeline->AddInterpFloat(VRPawnMoveUpCurve, ChangeValue);
	VRPawnMovementTimeline->SetTimelineFinishedFunc(FinishedEvent);

	if (VRPawnMovementTimeline)
	{
		VRPawnMovementTimeline->PlayFromStart();
	}

	UVREquipmentWorldSubsystem* TempEquipmentWorldSubSytem = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	if (TempEquipmentWorldSubSytem)
	{
		TempEquipmentWorldSubSytem->FEBMoveOrderSignature.AddDynamic(this, &ACVRPawn::HandleMovePlayerToFloor);
	}
}

void ACVRPawn::InitFloorData()
{
	// Init EB Related Floor Numuber
	CurrFloorNum = 3;
	PressedFloorNum = 0;

	TargetPlayerHeights.Add(194.0f);
	TargetPlayerHeights.Add(1104.0f);
	TargetPlayerHeights.Add(2084.0f);
}

void ACVRPawn::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);

	if (VRPawnMovementTimeline)
		VRPawnMovementTimeline->TickComponent(DeltaTimes, ELevelTick::LEVELTICK_TimeOnly, nullptr);

	ChairBody->SetRelativeRotation(
		FRotator(
			0.f,
			Camera->GetRelativeRotation().Yaw - 90.0f,
			0.f
		)
	);
}

void ACVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACVRPawn::SpawnHands()
{
	if (GetWorld())
	{
		if (LeftHandBPClass)
		{
			AActor* LeftHandObject = GetWorld()->SpawnActor<AVRHand>(LeftHandBPClass, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
			if (LeftHandObject)
			{
				LeftHandObject->SetOwner(this);
				LeftHandObject->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			}
		}

		if (RightHandBPClass)
		{
			AActor* RightHandObject = GetWorld()->SpawnActor<AVRHand>(RightHandBPClass, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
			if (RightHandObject)
			{
				RightHandObject->SetOwner(this);
				RightHandObject->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			}
		}
	}
}

void ACVRPawn::VRPawnMoveUpTLFunc(float Value)
{
	this->RootComponent->SetWorldLocation(FVector(0.f, 0.f, (Value * 1000.0f)));
}

void ACVRPawn::VRPawnMoveUpTLEndFunc()
{
	return;
}

void ACVRPawn::PlayerMovingUpAndDownInStage(uint8 InDir)
{
	// True : Up 
	if (InDir)
		this->RootComponent->SetWorldLocation(FVector(0.f, 0.f, 1000.0f));
	// False : Down
	else
		this->RootComponent->SetWorldLocation(FVector(0.f, 0.f, 2083.5f));
}

void ACVRPawn::HandleMovePlayerToFloor(FName InTag, int32 InTargetFloor)
{
	FLatentActionInfo EBTempLatentInfo;
	EBTempLatentInfo.CallbackTarget = this;
	EBTempLatentInfo.Linkage = 0;
	EBTempLatentInfo.UUID = 1003;

	if (InTag == TEXT("EB"))
	{
		PressedFloorNum = InTargetFloor;
		if (CurrFloorNum != PressedFloorNum)
		{
			CurrFloorNum = PressedFloorNum;
			float TempTargetFloorHeight = TargetPlayerHeights[InTargetFloor - 1];

			UKismetSystemLibrary::MoveComponentTo(
				this->RootComponent,
				FVector(0.f, 0.f, TempTargetFloorHeight),
				FRotator(0.f, 0.f, 0.f),
				true,
				true,
				5.0f,
				false,
				EMoveComponentAction::Move,
				EBTempLatentInfo
			);
		}
	}
}