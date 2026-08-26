#include "CoreCommon/VRPawn/CVRPawn.h"
#include "CoreCommon/VRPawn/Hand/VRHand.h"
#include "Camera/CameraComponent.h"
#include "IXRTrackingSystem.h"
#include "IHeadMountedDisplay.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreCommon/UMG/VRPawnHUD.h"
#include "CoreCommon/UMG/VRDialogueHUD.h"
#include "CoreObj/Manager/VRGameInstance.h"
#include "CoreObj/Manager/GameInstanceSubSystem/VRGameInstanceSubsystem.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreObj/Manager/GameInstanceSubSystem/VRDialogueManagerSubsystem.h"
#include "CoreObj/GameMode/VRGameMode.h"
#include "CoreObj/GameMode/VRLobbyGameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainActor/TowerBuilding.h"
#include "Components/WidgetComponent.h"

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

	VRPawnUpMovementTimeline = CreateDefaultSubobject<UTimelineComponent>("VRPawn_UpTLComp");
	if (VRPawnUpMovementTimeline)
	{
		VRPawnUpMovementTimeline->SetLooping(false);
		VRPawnUpMovementTimeline->SetTimelineLength(2.01f);
	}

	VRPawnDownMovementTimeline = CreateDefaultSubobject<UTimelineComponent>("VRPawn_DownTLComp");
	if (VRPawnDownMovementTimeline)
	{
		VRPawnDownMovementTimeline->SetLooping(false);
		VRPawnDownMovementTimeline->SetTimelineLength(2.01f);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_MoveUp(TEXT("/Game/VRContent/Blueprints/TimelineCurve/PlayerUpDown_Move_Curve.PlayerUpDown_Move_Curve"));
	if (CurveFinder_MoveUp.Succeeded())
	{
		VRPawnMoveUpCurve = CurveFinder_MoveUp.Object;
	}

	// Lobby Map In Setting Up&Down Move Timeline And CurveFloat Asset
	TL_VRPawnDownMoveInLobby = CreateDefaultSubobject<UTimelineComponent>("VRPawn_Down_MoveTLComp_InLobby");
	if (TL_VRPawnDownMoveInLobby)
	{
		TL_VRPawnDownMoveInLobby->SetLooping(false);
		TL_VRPawnDownMoveInLobby->SetTimelineLength(5.01f);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_LobbyDownMove(TEXT("/Game/VRContent/Blueprints/TimelineCurve/Lobby_PlayerDown_Move_Curve.Lobby_PlayerDown_Move_Curve"));
	if (CurveFinder_LobbyDownMove.Succeeded())
	{
		VRPawnLobbyDownMoveCurve = CurveFinder_LobbyDownMove.Object;
	}

	TL_VRPawnUpMoveInLobby = CreateDefaultSubobject<UTimelineComponent>("VRPawn_Up_MoveTLComp_InLobby");
	if (TL_VRPawnUpMoveInLobby)
	{
		TL_VRPawnUpMoveInLobby->SetLooping(false);
		TL_VRPawnUpMoveInLobby->SetTimelineLength(5.01f);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_LobbyUpMove(TEXT("/Game/VRContent/Blueprints/TimelineCurve/Lobby_PlayerUp_Move_Curve.Lobby_PlayerUp_Move_Curve"));
	if (CurveFinder_LobbyUpMove.Succeeded())
	{
		VRPawnLobbyUpMoveCurve = CurveFinder_LobbyUpMove.Object;
	}
	// ---

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ChairPlatform(TEXT("/Game/VRContent/Modeling/14_Lobby/SM_Platform.SM_Platform"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Lobby(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (ModelingFinder_ChairPlatform.Succeeded() && MaterialFinder_Lobby.Succeeded())
	{
		ChairPlatform->SetStaticMesh(ModelingFinder_ChairPlatform.Object);
		ChairPlatform->SetMaterial(0, MaterialFinder_Lobby.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ChairBody(TEXT("/Game/VRContent/Modeling/20_Stool(Chair)/SM_StoolChairBody.SM_StoolChairBody"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (ModelingFinder_ChairBody.Succeeded() && MaterialFinder_Main.Succeeded())
	{
		ChairBody->SetStaticMesh(ModelingFinder_ChairBody.Object);
		ChairBody->SetMaterial(0, MaterialFinder_Main.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_TowerHead(TEXT("/Game/VRContent/Modeling/24_Tower(Building_Tower)/SM_TowerHead.SM_TowerHead"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_TowerHead(TEXT("/Game/VRContent/Material/SRS_STAGE_TOWER.SRS_STAGE_TOWER"));
	if (ModelingFinder_TowerHead.Succeeded() && MaterialFinder_TowerHead.Succeeded())
	{
		ChairTowerHead->SetStaticMesh(ModelingFinder_TowerHead.Object);
		ChairTowerHead->SetMaterial(0, MaterialFinder_TowerHead.Object);
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

	static ConstructorHelpers::FClassFinder<UVRPawnHUD> HUDFinder_VRPawnHUD(TEXT("/Game/VRContent/Blueprints/UserWidget/BPVRPawnHUD.BPVRPawnHUD_C"));
	if (HUDFinder_VRPawnHUD.Succeeded())
		VRPawnHUDWidgetClass = HUDFinder_VRPawnHUD.Class;

	static ConstructorHelpers::FClassFinder<UVRDialogueHUD> HUDFinder_VRDialogueHUD(TEXT("/Game/VRContent/Blueprints/UserWidget/BPVRDialogueHUD.BPVRDialogueHUD_C"));
	if (HUDFinder_VRDialogueHUD.Succeeded())
		VRDialogueHUDWidgetClass = HUDFinder_VRDialogueHUD.Class;

	HMD = CreateDefaultSubobject<UStaticMeshComponent>("SM_HMD");
	if (HMD)
	{
		HMD->SetupAttachment(Camera);
		HMD->ComponentTags.Add(FName("VRPawn"));
		HMD->SetCollisionProfileName(FName("OverlapAll"));
		HMD->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		HMD->SetHiddenInGame(true);
		HMD->SetRelativeLocation(FVector(11.0f, 0.0f, 0.0f));

		static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_HMD(TEXT("/Engine/VREditor/Devices/Generic/GenericHMD.GenericHMD"));
		if (ModelingFinder_HMD.Succeeded())
			HMD->SetStaticMesh(ModelingFinder_HMD.Object);
	}

	SM_MaskPlane = CreateDefaultSubobject<UStaticMeshComponent>("SM_Plane");
	if (SM_MaskPlane)
	{
		SM_MaskPlane->SetupAttachment(Camera);
		SM_MaskPlane->SetRelativeLocation(FVector(25.0f, 0.f, 0.f));
		SM_MaskPlane->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		SM_MaskPlane->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		SM_MaskPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SM_MaskPlane->SetCastShadow(false);

		static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Plane(TEXT("/Game/VRContent/Modeling/13_Keypad/Periscope_SM_UI.Periscope_SM_UI"));
		static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Scope(TEXT("/Game/VRContent/Material/SRS_Stage_Scope_Mat_Inst.SRS_Stage_Scope_Mat_Inst"));
		if (ModelingFinder_Plane.Succeeded() && MatFinder_Scope.Succeeded())
		{
			SM_MaskPlane->SetStaticMesh(ModelingFinder_Plane.Object);
			MaskMI = MatFinder_Scope.Object;
		}
	}

	HUDWidgetComp = CreateDefaultSubobject<UWidgetComponent>("VRPawnHUDWidget");
	if (HUDWidgetComp)
	{
		HUDWidgetComp->SetupAttachment(Root);
		HUDWidgetComp->SetRelativeLocation(FVector(290.f, 0.f, -25.f));
		HUDWidgetComp->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
		HUDWidgetComp->SetWidgetSpace(EWidgetSpace::World);
		HUDWidgetComp->SetWidgetClass(HUDFinder_VRPawnHUD.Class);
		HUDWidgetComp->SetDrawSize(FVector2D(500.0f, 500.0f));
		HUDWidgetComp->SetTwoSided(true);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_PassThrough(TEXT("/Engine/EngineMaterials/Widget3DPassThrough_Inst.Widget3DPassThrough_Inst"));

	VRDialogueHUDWidgetComp = CreateDefaultSubobject<UWidgetComponent>("VRDialogueHUDWidgetComp");
	if (VRDialogueHUDWidgetComp)
	{
		VRDialogueHUDWidgetComp->SetupAttachment(Camera);
		VRDialogueHUDWidgetComp->SetRelativeLocation(FVector(328.6f, 0.f, -17.0f));
		VRDialogueHUDWidgetComp->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
		VRDialogueHUDWidgetComp->SetWidgetSpace(EWidgetSpace::World);
		VRDialogueHUDWidgetComp->SetWidgetClass(HUDFinder_VRDialogueHUD.Class);
		VRDialogueHUDWidgetComp->SetDrawSize(FVector2D(1000.0f, 1000.0f));
		VRDialogueHUDWidgetComp->SetTwoSided(true);
		VRDialogueHUDWidgetComp->TranslucencySortPriority = 9999;
		if (MatFinder_PassThrough.Succeeded())
		{
			VRDialogueHUDWidgetComp->SetMaterial(0, MatFinder_PassThrough.Object);
		}
	}

	this->GetCharacterMovement()->GravityScale = 0.0f;
	this->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Camera->ComponentTags.Add(FName("VRPawn"));

	InitFloorData();
}

void ACVRPawn::BeginPlay()
{
	Super::BeginPlay();

	if (!MaskMID_0Index && !MaskMID_1Index)
	{
		MaskMID_0Index = SM_MaskPlane->CreateDynamicMaterialInstance(0, MaskMI);
		MaskMID_1Index = SM_MaskPlane->CreateDynamicMaterialInstance(1, MaskMI);

		if (MaskMID_0Index && MaskMID_1Index)
		{
			MaskMID_0Index->SetScalarParameterValue(FName("MaskOpacity"), 0.0f);
			MaskMID_1Index->SetScalarParameterValue(FName("MaskOpacity"), 0.0f);
		}
		SM_MaskPlane->SetVisibility(false);
		UE_LOG(LogTemp, Warning, TEXT("Mask Related Setting Success!!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Mask Related Setting Already!"));
	}

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

	UVREquipmentWorldSubsystem* TempEquipmentWorldSubSytem = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	check(TempEquipmentWorldSubSytem);

	if (HUDWidgetComp)
	{
		HUDWidgetInstance = Cast<UVRPawnHUD>(HUDWidgetComp->GetUserWidgetObject());
	}

	if (VRDialogueHUDWidgetComp)
	{
		VRDialogueHUDWidgetInstance = Cast<UVRDialogueHUD>(VRDialogueHUDWidgetComp->GetUserWidgetObject());
	}

	AGameModeBase* CurrGM = UGameplayStatics::GetGameMode(GetWorld());
	check(CurrGM);

	if (Cast<AVRLobbyGameMode>(CurrGM))
	{
		mVRLobbyGMRef = Cast<AVRLobbyGameMode>(CurrGM);
		check(mVRLobbyGMRef);

		FOnTimelineFloat LobbyUpMoveChangeValue;
		FOnTimelineEvent LobbyUpMoveFinishedEvent;
		LobbyUpMoveChangeValue.BindUFunction(this, FName("VRPawnUpMoveInLobbyTLFunc"));
		LobbyUpMoveFinishedEvent.BindUFunction(this, FName("VRPawnUpMoveInLobbyTLEndFunc"));
		TL_VRPawnUpMoveInLobby->AddInterpFloat(VRPawnLobbyUpMoveCurve, LobbyUpMoveChangeValue);
		TL_VRPawnUpMoveInLobby->SetTimelineFinishedFunc(LobbyUpMoveFinishedEvent);

		FOnTimelineFloat LobbyDownMoveChangeValue;
		FOnTimelineEvent LobbyDownMoveFinishedEvent;
		LobbyDownMoveChangeValue.BindUFunction(this, FName("VRPawnDownMoveInLobbyTLFunc"));
		LobbyDownMoveFinishedEvent.BindUFunction(this, FName("VRPawnDownMoveInLobbyTLEndFunc"));
		TL_VRPawnDownMoveInLobby->AddInterpFloat(VRPawnLobbyDownMoveCurve, LobbyDownMoveChangeValue);
		TL_VRPawnDownMoveInLobby->SetTimelineFinishedFunc(LobbyDownMoveFinishedEvent);

		TL_VRPawnDownMoveInLobby->PlayFromStart();
		HideTowerHeadMesh(true);

		TempEquipmentWorldSubSytem->FLobbyGameStartSignature.BindUObject(this, &ACVRPawn::GameStartInLobbyEvent);
	}
	else if (Cast<AVRGameMode>(CurrGM))
	{
		FOnTimelineFloat UpMoveChangeValue;
		FOnTimelineEvent UpMoveFinishedEvent;
		UpMoveChangeValue.BindUFunction(this, FName("VRPawnMoveUpTLFunc"));
		UpMoveFinishedEvent.BindUFunction(this, FName("VRPawnMoveUpTLEndFunc"));
		VRPawnUpMovementTimeline->AddInterpFloat(VRPawnMoveUpCurve, UpMoveChangeValue);
		VRPawnUpMovementTimeline->SetTimelineFinishedFunc(UpMoveFinishedEvent);

		FOnTimelineFloat DownMoveChangeValue;
		FOnTimelineEvent DownMoveFinishedEvent;
		DownMoveChangeValue.BindUFunction(this, FName("VRPawnMoveDownTLFunc"));
		DownMoveFinishedEvent.BindUFunction(this, FName("VRPawnMoveDownTLEndFunc"));
		VRPawnDownMovementTimeline->AddInterpFloat(VRPawnMoveUpCurve, DownMoveChangeValue);
		VRPawnDownMovementTimeline->SetTimelineFinishedFunc(DownMoveFinishedEvent);

		if (VRPawnUpMovementTimeline)
		{
			VRPawnUpMovementTimeline->PlayFromStart();
			HideTowerHeadMesh(false);
		}

		TempEquipmentWorldSubSytem->FEBMoveOrderSignature.AddDynamic(this, &ACVRPawn::HandleMovePlayerToFloor);
		TempEquipmentWorldSubSytem->FGameStartSignature.AddDynamic(this, &ACVRPawn::HandleVRPawnReceivceByGTW);
	}

	GetWorld()->GetTimerManager().SetTimer(
		DebuggingTimer,
		this,
		&ACVRPawn::Debuggigng,
		3.0f,
		false
	);
}

void ACVRPawn::Debuggigng()
{
	UVRDialogueManagerSubsystem* TempDialogueMgr = GetWorld()->GetGameInstance()->GetSubsystem<UVRDialogueManagerSubsystem>();

	FText TempText = FText::FromString(TEXT("Ugh... I must have dozed off for a moment."));

	TempDialogueMgr->StartDialogue();
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

void ACVRPawn::HideTowerHeadMesh(bool bIsHideFlag)
{
	ChairTowerHead->SetHiddenInGame(bIsHideFlag);
}

void ACVRPawn::GameStartInLobbyEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("In Lobby Game Start Logic Call Part!"));
	TL_VRPawnUpMoveInLobby->PlayFromStart();
}

void ACVRPawn::HandleVRPawnReceivceByGTW(bool InFlag)
{
	if (InFlag)
	{
		UE_LOG(LogTemp, Log, TEXT("VRPawn HUD Timer Start By GTWLever"));
		if (IsValid(HUDWidgetInstance))
		{
			UKismetSystemLibrary::K2_UnPauseTimer(HUDWidgetInstance, TEXT("UpdateTimerWidget"));
		}
	}
}

void ACVRPawn::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);

	if (VRPawnUpMovementTimeline)
		VRPawnUpMovementTimeline->TickComponent(DeltaTimes, ELevelTick::LEVELTICK_TimeOnly, nullptr);

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

// VRPawn Move Up Function In MainMap
void ACVRPawn::VRPawnMoveUpTLFunc(float Value)
{
	this->RootComponent->SetWorldLocation(FVector(0.f, 0.f, (Value * 1000.0f)));
}
// VRPawn Move Up End Function In MainMap
void ACVRPawn::VRPawnMoveUpTLEndFunc()
{
	//UE_LOG(LogTemp, Log, TEXT("VRPawn MoveUp End In MainMap"));
	return;
}
// VRPawn Move Down Function In MainMap
void ACVRPawn::VRPawnMoveDownTLFunc(float Value)
{
	this->GetRootComponent()->SetWorldLocation(FVector(0.f, 0.f, (Value * 1000.0f)));
}
// VRPawn Move Down End Function In MainMap
void ACVRPawn::VRPawnMoveDownTLEndFunc()
{
	// UE_LOG(LogTemp, Log, TEXT("LobbyMap Open!"));
	UGameplayStatics::OpenLevel(GetWorld(), FName("LobbyMap"));
	return;
}
// VRPawn Move Up Function In LobbyMap
void ACVRPawn::VRPawnUpMoveInLobbyTLFunc(float Value)
{
	this->GetRootComponent()->SetWorldLocation(FVector(0.f, 0.f, (Value * 1000.0f)));
}
// VRPawn Move Up End Function In LobbyMap
void ACVRPawn::VRPawnUpMoveInLobbyTLEndFunc()
{
	UE_LOG(LogTemp, Log, TEXT("MainMap Open!"));

	mVRLobbyGMRef->HandleOpenMainMap();
}
// VRPawn Move Down Function In LobbyMap
void ACVRPawn::VRPawnDownMoveInLobbyTLFunc(float Value)
{
	this->GetRootComponent()->SetWorldLocation(FVector(0.f, 0.f, (Value * 1000.0f)));
}
// VRPawn Move Down End Function In LobbyMap
void ACVRPawn::VRPawnDownMoveInLobbyTLEndFunc()
{
	UE_LOG(LogTemp, Log, TEXT("LobbyMap Arrived!"));

	mVRLobbyGMRef->CheckGameResult();
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

void ACVRPawn::HandleDownMovePlayer()
{
	UVREquipmentWorldSubsystem* TempVREquipmentWorldSubSystemRef = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	ATowerBuilding* TempTowerObj = nullptr;

	if (!ensure(TempVREquipmentWorldSubSystemRef)) return;

	for (TScriptInterface<IIEquipmentInitInterface> Equip : TempVREquipmentWorldSubSystemRef->GetEquipmentArr())
	{
		IIEquipmentInitInterface* IEquipPtr = Equip.GetInterface();
		TempTowerObj = Cast<ATowerBuilding>(IEquipPtr);
		if (TempTowerObj)
			break;
		else
			continue;
	}

	this->GetRootComponent()->SetWorldLocation(FVector(0.f, 0.f, 2080.0f), false, nullptr, ETeleportType::TeleportPhysics);
	TempTowerObj->GetRootComponent()->SetWorldLocation(FVector(0.f, 0.f, 790.f), false, nullptr, ETeleportType::TeleportPhysics);

	VRPawnDownMovementTimeline->ReverseFromEnd();
}

void ACVRPawn::HandleMaskOpacity(float OpacityValue)
{
	SM_MaskPlane->SetVisibility(true);
	if (MaskMID_0Index && MaskMID_1Index)
	{
		MaskMID_0Index->SetScalarParameterValue(FName("MaskOpacity"), OpacityValue * 2.0f);
		MaskMID_1Index->SetScalarParameterValue(FName("MaskOpacity"), OpacityValue);
	}
}
