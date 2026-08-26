#include "MainActor/ALobbyRoom.h"
#include "EquipmentActor/Spawned_Actors/ATape.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "CoreCommon/Component/UGrabComp.h"
#include "Components/TimelineComponent.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreObj/Manager/GameInstanceSubSystem/LevelSequenceManagerSubsystem.h"
#include "CoreObj/GameMode/VRLobbyGameMode.h"
#include "CoreCommon/VRPawn/CVRPawn.h"
#include "LevelSequence.h"

AALobbyRoom::AALobbyRoom()
{
	SC_MainRoot = CreateDefaultSubobject<USceneComponent>("SceneRootComp");
	if (SC_MainRoot)
	{
		this->SetRootComponent(SC_MainRoot);
	}

	SM_SpaceEV = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_SpaceEV");
	if (SM_SpaceEV)
	{
		SM_SpaceEV->SetupAttachment(SC_MainRoot);
		SM_SpaceEV->SetRelativeScale3D(FVector(0.8f));
		SM_SpaceEV->SetCollisionProfileName(FName(TEXT("BlockAll")));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_SpaceEV(TEXT("/Game/VRContent/Modeling/14_Lobby/SM_SpasceEV.SM_SpasceEV"));
		if (SMFinder_SpaceEV.Succeeded())
			SM_SpaceEV->SetStaticMesh(SMFinder_SpaceEV.Object);
	}

	SM_Cup = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_Cup");
	if (SM_Cup)
	{
		SM_Cup->SetupAttachment(SC_MainRoot);
		SM_Cup->SetRelativeLocation(FVector((47.4f, -91.5f, 62.0f)));
		SM_Cup->SetRelativeRotation(FRotator(0.f, -90.0f, 0.f));
		SM_Cup->SetRelativeScale3D(FVector(0.8f));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Cup(TEXT("/Game/VRContent/Modeling/14_Lobby/SM_Cup.SM_Cup"));
		if (SMFinder_Cup.Succeeded())
			SM_Cup->SetStaticMesh(SMFinder_Cup.Object);
	}

	SM_RoomWall = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_Wall");
	if (SM_RoomWall)
	{
		SM_RoomWall->SetupAttachment(SC_MainRoot);
		SM_RoomWall->SetRelativeScale3D(FVector(0.8f));
		SM_RoomWall->SetCollisionProfileName(TEXT("BlockAll"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Wall(TEXT("/Game/VRContent/Modeling/14_Lobby/SM_RoomWall.SM_RoomWall"));
		if (SMFinder_Wall.Succeeded())
			SM_RoomWall->SetStaticMesh(SMFinder_Wall.Object);
	}

	SM_RoomStuff = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_Stuff");
	if (SM_RoomStuff)
	{
		SM_RoomStuff->SetupAttachment(SC_MainRoot);
		SM_RoomStuff->SetRelativeScale3D(FVector(0.8f));
		SM_RoomStuff->SetCollisionProfileName(TEXT("BlockAll"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Stuff(TEXT("/Game/VRContent/Modeling/14_Lobby/SM_LobbyStuff.SM_LobbyStuff"));
		if (SMFinder_Stuff.Succeeded())
			SM_RoomStuff->SetStaticMesh(SMFinder_Stuff.Object);
	}
	
	// StartLeber Body Static Mesh Component
	ActorBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_StartLever");
	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetupAttachment(SC_MainRoot);
		ActorBaseMesh->SetRelativeLocation(FVector(-23.861886f, -94.270769f, 50.276919));
		ActorBaseMesh->SetRelativeScale3D(FVector(0.8f));
		ActorBaseMesh->SetCollisionProfileName(TEXT("BlockAll"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_StartLever(TEXT("/Game/VRContent/Modeling/14_Lobby/SM_StartLever_Body.SM_StartLever_Body"));
		if (SMFinder_StartLever.Succeeded())
		{
			ActorBaseMesh->SetStaticMesh(SMFinder_StartLever.Object);
		}
	}

	SM_StartLeverHandle = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_StartLever_Handle");
	if (SM_StartLeverHandle)
	{
		SM_StartLeverHandle->SetupAttachment(ActorBaseMesh);
		SM_StartLeverHandle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_StartLeverHandle(TEXT("/Game/VRContent/Modeling/14_Lobby/SM_StartLever_Handle.SM_StartLever_Handle"));
		if (SMFinder_StartLeverHandle.Succeeded())
			SM_StartLeverHandle->SetStaticMesh(SMFinder_StartLeverHandle.Object);
	}

	CL_Handle = CreateDefaultSubobject<UCapsuleComponent>("HandleColComp");
	if (CL_Handle)
	{
		CL_Handle->SetupAttachment(SM_StartLeverHandle);
		CL_Handle->SetRelativeLocation(FVector(63.0f, 0.0f, 50.0f));
		CL_Handle->SetCollisionProfileName(TEXT("OverlapAll"));
		CL_Handle->SetCapsuleRadius(2.25f, true);
		CL_Handle->SetCapsuleHalfHeight(5.0f);
		CL_Handle->SetHiddenInGame(false); // Debug
		CL_Handle->OnComponentBeginOverlap.AddDynamic(this, &AALobbyRoom::SLHandleOverlapBegin);
		CL_Handle->OnComponentEndOverlap.AddDynamic(this, &AALobbyRoom::SLHandleFOverlapEnd);
	}

	if (GC)
	{
		GC->SetupAttachment(SM_StartLeverHandle);
		GC->EEGrabType = EGrabType::HandToObj;
	}

	CL_TapeTarget = CreateDefaultSubobject<UBoxComponent>("TapeTargetPathColComp");
	if (CL_TapeTarget)
	{
		CL_TapeTarget->SetupAttachment(ActorBaseMesh);
		CL_TapeTarget->SetRelativeLocation(FVector(26.5f, 13.0f, 0.5f));
		CL_TapeTarget->SetBoxExtent(FVector(20.2f, 15.0f, 3.1f));
		CL_TapeTarget->OnComponentBeginOverlap.AddDynamic(this, &AALobbyRoom::TapePathOverlapBegin);
		CL_TapeTarget->OnComponentEndOverlap.AddDynamic(this, &AALobbyRoom::TapePathOverlapEnd);
	}

	SM_SL_Roller01 = CreateDefaultSubobject<UStaticMeshComponent>("SM_Roller01_Comp");
	SM_SL_Roller02 = CreateDefaultSubobject<UStaticMeshComponent>("SM_Roller02_Comp");
	SM_SL_Roller03 = CreateDefaultSubobject<UStaticMeshComponent>("SM_Roller03_Comp");
	if (SM_SL_Roller01 && SM_SL_Roller02 && SM_SL_Roller03)
	{
		float TempY = 16.2f;
		float TempZ = 21.8f;
		SM_SL_Roller01->SetupAttachment(ActorBaseMesh);
		SM_SL_Roller01->SetRelativeLocation(FVector(8.8f, TempY, TempZ));
		SM_SL_Roller01->SetRelativeScale3D(FVector(1.05f));

		SM_SL_Roller02->SetupAttachment(ActorBaseMesh);
		SM_SL_Roller02->SetRelativeLocation(FVector(26.4f, TempY, TempZ));
		SM_SL_Roller02->SetRelativeScale3D(FVector(1.05f));

		SM_SL_Roller03->SetupAttachment(ActorBaseMesh);
		SM_SL_Roller03->SetRelativeLocation(FVector(44.1f, TempY, TempZ));
		SM_SL_Roller03->SetRelativeScale3D(FVector(1.05f));
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Roller001(TEXT("/Game/VRContent/Modeling/14_Lobby/SM_StartLever_Roller001.SM_StartLever_Roller001"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Roller002(TEXT("/Game/VRContent/Modeling/14_Lobby/SM_StartLever_Roller002.SM_StartLever_Roller002"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Roller003(TEXT("/Game/VRContent/Modeling/14_Lobby/SM_StartLever_Roller003.SM_StartLever_Roller003"));
	if (SMFinder_Roller001.Succeeded())
		SM_SL_Roller01->SetStaticMesh(SMFinder_Roller001.Object);
	if (SMFinder_Roller002.Succeeded())
		SM_SL_Roller02->SetStaticMesh(SMFinder_Roller002.Object);
	if (SMFinder_Roller003.Succeeded())
		SM_SL_Roller03->SetStaticMesh(SMFinder_Roller003.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Lobby(TEXT("/Game/VRContent/Material/SRS_LOBBY.SRS_LOBBY"));
	if (MatFinder_Lobby.Succeeded())
	{
		SM_SpaceEV->SetMaterial(0, MatFinder_Lobby.Object);
		SM_Cup->SetMaterial(0, MatFinder_Lobby.Object);
		SM_RoomWall->SetMaterial(0, MatFinder_Lobby.Object);
		SM_RoomStuff->SetMaterial(0, MatFinder_Lobby.Object);
		ActorBaseMesh->SetMaterial(0, MatFinder_Lobby.Object);
		SM_StartLeverHandle->SetMaterial(0, MatFinder_Lobby.Object);
	}

	AtFirstHandleRot = FRotator::ZeroRotator;
	bIsHanding = false;

	TL_TapeMove = CreateDefaultSubobject<UTimelineComponent>("TapeMoveInTLComp");
	if (TL_TapeMove)
	{
		TL_TapeMove->SetLooping(false);
		TL_TapeMove->SetTimelineLength(2.01f);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CFFinder_MoveIn(TEXT("/Game/VRContent/Blueprints/TimelineCurve/LobbyRoom_Tape_MoveIn_Curve.LobbyRoom_Tape_MoveIn_Curve"));
	if (CFFinder_MoveIn.Succeeded())
	{
		CF_TapeMoveIn = CFFinder_MoveIn.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Roller(TEXT("/Game/VRContent/Material/SRS_Lobby_StartLever_Rollers.SRS_Lobby_StartLever_Rollers"));
	if (MatFinder_Roller.Succeeded())
	{
		SM_SL_Roller01->SetMaterial(0, MatFinder_Roller.Object);
		SM_SL_Roller02->SetMaterial(0, MatFinder_Roller.Object);
		SM_SL_Roller03->SetMaterial(0, MatFinder_Roller.Object);
	}

	static ConstructorHelpers::FObjectFinder<ULevelSequence> LQFinder_Debugging(TEXT("/Game/VRContent/LevelSequence/DebuggingLQ.DebuggingLQ"));
	if (LQFinder_Debugging.Succeeded())
		LQ_Roller = LQFinder_Debugging.Object;
}

void AALobbyRoom::BeginPlay()
{
	Super::BeginPlay();

	AtFirstHandleRot = SM_StartLeverHandle->GetRelativeRotation();

	FOnTimelineFloat TapeMoveProgressFunc;
	FOnTimelineEvent TapeMoveFinishedEvent;

	TapeMoveProgressFunc.BindUFunction(this, FName("TapeMovePlayEvent"));
	TapeMoveFinishedEvent.BindUFunction(this, FName("TapeMoveFinishedEvent"));

	TL_TapeMove->AddInterpFloat(CF_TapeMoveIn, TapeMoveProgressFunc);
	TL_TapeMove->SetTimelineFinishedFunc(TapeMoveFinishedEvent);

	ActorBaseMesh->SetRelativeLocation(FVector(-23.8f, -47.0f, 62.9f));// Debug
	CL_TapeTarget->SetBoxExtent(FVector(20.2f, 15.0f, 3.1f));
	bIsTapeMoveingFlag = 0;
}

void AALobbyRoom::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, AVRHand* InGrabbingHand)
{
	//StartLobbyRoomLQ();
	TempMCRef = &InMCRef;
	AdjustVecNRot(&InMCRef);
	if (bIsHanding)
	{
		HVRSoundPlayer::PlaySoundEffect(this, SFX_HeavyGrab, this->GetRootComponent()->GetComponentLocation());
		GetWorld()->GetTimerManager().SetTimer(
			StartLeverMoveTimer,
			this,
			&AALobbyRoom::UpdateStartLever,
			0.01f,
			true
		);
	}
}

void AALobbyRoom::OnDropped()
{
	GetWorld()->GetTimerManager().PauseTimer(StartLeverMoveTimer);
	GetWorld()->GetTimerManager().ClearTimer(StartLeverMoveTimer);
	TempMCRef = nullptr;
	SM_StartLeverHandle->SetRelativeRotation(AtFirstHandleRot);
}

void AALobbyRoom::SLHandleOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("HandCLBox")))
	{
		bIsHanding = true;
	}
}

void AALobbyRoom::SLHandleFOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("HandCLBox")))
	{
		bIsHanding = false;
	}
}

void AALobbyRoom::TapePathOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("Tape")) && IsValid(OtherActor) && !IsValid(NewTape))
	{
		NewTape = Cast<AATape>(OtherActor);
		check(NewTape);
		NewTape->GC->GCTryRelease();

		if (NewTape->GetRootComponent()->AttachToComponent(CL_TapeTarget, FAttachmentTransformRules::SnapToTargetNotIncludingScale))
		{
			bIsTapeMoveingFlag = 1;
			NewTape->HandleDontGrabPhysics(1);
			TL_TapeMove->PlayFromStart();
		}
	}
}

void AALobbyRoom::TapePathOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsTapeMoveingFlag) return;
	if (OtherComp && OtherComp->ComponentHasTag(FName("Tape")) && IsValid(NewTape))
		NewTape = nullptr;
}

void AALobbyRoom::TapeMovePlayEvent(float Value)
{
	if (IsValid(NewTape))
	{
		NewTape->GetRootComponent()->SetRelativeLocation(FVector(0.f, Value * 10.0f, 0.f), false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void AALobbyRoom::TapeMoveFinishedEvent()
{
	if (IsValid(NewTape))
	{
		bIsTapeMoveingFlag = 0;
		NewTape->HandleDontGrabPhysics(0);
		UE_LOG(LogTemp, Log, TEXT("Tape Move In End!"));
	}
}

void AALobbyRoom::AdjustVecNRot(UMotionControllerComponent* InMC)
{
	if (InMC->ComponentHasTag(FName(TEXT("RightMC"))))
	{
		GC->SetRelativeLocationAndRotation(FVector(65.0f, 7.0f, 50.0f), FRotator(90.0f, 0.0f, 180.f), false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		GC->SetRelativeLocationAndRotation(FVector(61.0f, 7.0f, 50.0f), FRotator(-90.0f, 0.0f, 180.f), false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void AALobbyRoom::UpdateStartLever()
{
	FTransform FTransform_Between_MCAndGC_MakeRelative = TempMCRef->GetComponentTransform().GetRelativeTransform(GC->GetComponentTransform());

	FVector HandleMoveDir = (GC->GetComponentLocation()) - (FTransform_Between_MCAndGC_MakeRelative.GetLocation());

	float TargetRoll = FMath::Clamp(SM_StartLeverHandle->GetRelativeRotation().Roll + HandleMoveDir.Y, 0.0f, 89.0f);

	SM_StartLeverHandle->SetRelativeRotation(FRotator(0.0f, 0.0f, TargetRoll));

	if (SM_StartLeverHandle->GetRelativeRotation().Roll >= 89.0f)
	{
		LeverOnGameStartEvent();
	}
}

void AALobbyRoom::LeverOnGameStartEvent()
{
	this->OnDropped();
	UE_LOG(LogTemp, Warning, TEXT("In Lobby Game Start Logic Call Part!"));
	
	StartLQInLobbyRoom();

	/*if (APlayerController* mPC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (ACVRPawn* TempVRPawn = Cast<ACVRPawn>(mPC->GetPawn()))
		{
			TempVRPawn->GameStartInLobbyEvent();
		}
	}*/
}

void AALobbyRoom::StartLQInLobbyRoom()
{
	ULevelSequenceManagerSubsystem* LQMgr = GetWorld()->GetGameInstance()->GetSubsystem<ULevelSequenceManagerSubsystem>();
	if (LQMgr)
	{
		FOnSequenceFinishedSignature CallBack;
		CallBack.BindDynamic(this, &AALobbyRoom::OnLobbyRoomLQDone);
		LQMgr->PlaySequence(LQ_Roller, CallBack);
	}
}

void AALobbyRoom::OnLobbyRoomLQDone()
{
	// Player Lobby Up Move Part
	if (APlayerController* mPC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (ACVRPawn* TempVRPawn = Cast<ACVRPawn>(mPC->GetPawn()))
		{
			TempVRPawn->GameStartInLobbyEvent();
		}
	}
}
