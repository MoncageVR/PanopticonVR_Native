// Fill out your copyright notice in the Description page of Project Settings.


#include "BPMainActorBase/ALobbyRoom.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Core/Component/UGrabComp.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"

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
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_SpaceEV(TEXT("/Game/VRContent/Modeling/LobbyRoom/SpaceEV.SpaceEV"));
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
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Cup(TEXT("/Game/VRContent/Modeling/LobbyRoom/Cup.Cup"));
		if (SMFinder_Cup.Succeeded())
			SM_Cup->SetStaticMesh(SMFinder_Cup.Object);
	}

	SC_GlassRoot = CreateDefaultSubobject<USceneComponent>("GlassSceneRootComp");
	if (SC_GlassRoot)
	{
		SC_GlassRoot->SetupAttachment(SC_MainRoot);
	}

	SM_Glass001 = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_Monitor_Glass1");
	if (SM_Glass001)
	{
		SM_Glass001->SetupAttachment(SC_GlassRoot);
		SM_Glass001->SetRelativeScale3D(FVector(0.8f));
		SM_Glass001->SetCollisionProfileName(TEXT("BlockAll"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Glass1(TEXT("/Game/VRContent/Modeling/LobbyRoom/LobbyMonitorGlass001.LobbyMonitorGlass001"));
		if (SMFinder_Glass1.Succeeded())
			SM_Glass001->SetStaticMesh(SMFinder_Glass1.Object);
	}

	SM_Glass002 = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_Monitor_Glass2");
	if (SM_Glass002)
	{
		SM_Glass002->SetupAttachment(SC_GlassRoot);
		SM_Glass002->SetRelativeScale3D(FVector(0.8f));
		SM_Glass002->SetCollisionProfileName(TEXT("BlockAll"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Glass2(TEXT("/Game/VRContent/Modeling/LobbyRoom/LobbyMonitorGlass002.LobbyMonitorGlass002"));
		if (SMFinder_Glass2.Succeeded())
			SM_Glass002->SetStaticMesh(SMFinder_Glass2.Object);
	}

	SM_Glass003 = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_Monitor_Glass3");
	if (SM_Glass003)
	{
		SM_Glass003->SetupAttachment(SC_GlassRoot);
		SM_Glass003->SetRelativeScale3D(FVector(0.8f));
		SM_Glass003->SetCollisionProfileName(TEXT("BlockAll"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Glass3(TEXT("/Game/VRContent/Modeling/LobbyRoom/LobbyMonitorGlass003.LobbyMonitorGlass003"));
		if (SMFinder_Glass3.Succeeded())
			SM_Glass003->SetStaticMesh(SMFinder_Glass3.Object);
	}

	SM_Glass004 = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_Monitor_Glass4");
	if (SM_Glass004)
	{
		SM_Glass004->SetupAttachment(SC_GlassRoot);
		SM_Glass004->SetRelativeScale3D(FVector(0.8f));
		SM_Glass004->SetCollisionProfileName(TEXT("BlockAll"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Glass4(TEXT("/Game/VRContent/Modeling/LobbyRoom/LobbyMonitorGlass004.LobbyMonitorGlass004"));
		if (SMFinder_Glass4.Succeeded())
			SM_Glass004->SetStaticMesh(SMFinder_Glass4.Object);
	}

	SM_Glass005 = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_Monitor_Glass5");
	if (SM_Glass005)
	{
		SM_Glass005->SetupAttachment(SC_GlassRoot);
		SM_Glass005->SetRelativeScale3D(FVector(0.8f));
		SM_Glass005->SetCollisionProfileName(TEXT("BlockAll"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Glass5(TEXT("/Game/VRContent/Modeling/LobbyRoom/LobbyMonitorGlass005.LobbyMonitorGlass005"));
		if (SMFinder_Glass5.Succeeded())
			SM_Glass005->SetStaticMesh(SMFinder_Glass5.Object);
	}

	SM_Glass006 = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_Monitor_Glass6");
	if (SM_Glass006)
	{
		SM_Glass006->SetupAttachment(SC_GlassRoot);
		SM_Glass006->SetRelativeScale3D(FVector(0.8f));
		SM_Glass006->SetCollisionProfileName(TEXT("BlockAll"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Glass6(TEXT("/Game/VRContent/Modeling/LobbyRoom/LobbyMonitorGlass006.LobbyMonitorGlass006"));
		if (SMFinder_Glass6.Succeeded())
			SM_Glass006->SetStaticMesh(SMFinder_Glass6.Object);
	}

	SM_RoomWall = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_Wall");
	if (SM_RoomWall)
	{
		SM_RoomWall->SetupAttachment(SC_MainRoot);
		SM_RoomWall->SetRelativeScale3D(FVector(0.8f));
		SM_RoomWall->SetCollisionProfileName(TEXT("BlockAll"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Wall(TEXT("/Game/VRContent/Modeling/LobbyRoom/RoomWall.RoomWall"));
		if (SMFinder_Wall.Succeeded())
			SM_RoomWall->SetStaticMesh(SMFinder_Wall.Object);
	}

	SM_RoomStuff = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_Stuff");
	if (SM_RoomStuff)
	{
		SM_RoomStuff->SetupAttachment(SC_MainRoot);
		SM_RoomStuff->SetRelativeScale3D(FVector(0.8f));
		SM_RoomStuff->SetCollisionProfileName(TEXT("BlockAll"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Stuff(TEXT("/Game/VRContent/Modeling/LobbyRoom/LobbyStuff.LobbyStuff"));
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
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_StartLever(TEXT("/Game/VRContent/Modeling/LobbyRoom/StartLever.StartLever"));
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
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_StartLeverHandle(TEXT("/Game/VRContent/Modeling/LobbyRoom/StartLeverHandle.StartLeverHandle"));
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
		CL_TapeTarget->ComponentTags.Add("TapeDispenser");
	}

	SM_LavaLamp = CreateDefaultSubobject<UStaticMeshComponent>("SM_Lobby_Lavalamp");
	if (SM_LavaLamp)
	{
		SM_LavaLamp->SetupAttachment(SC_MainRoot);
		SM_LavaLamp->SetRelativeScale3D(FVector(0.8f));
		SM_LavaLamp->SetCollisionProfileName(FName(TEXT("BlockAll")));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Lavalamp(TEXT("/Game/VRContent/Modeling/LobbyRoom/LavaLamp.LavaLamp"));
		if (SMFinder_Lavalamp.Succeeded())
			SM_LavaLamp->SetStaticMesh(SMFinder_Lavalamp.Object);
	}

	AtFirstHandleRot = FRotator::ZeroRotator;
	bIsHanding = false;
}

void AALobbyRoom::BeginPlay()
{
	Super::BeginPlay();

	AtFirstHandleRot = SM_StartLeverHandle->GetRelativeRotation();
}

void AALobbyRoom::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, AVRHand* InGrabbingHand)
{
	TempMCRef = &InMCRef;
	AdjustVecNRot(&InMCRef);
	if (bIsHanding)
	{
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
	//UE_LOG(LogTemp, Warning, TEXT("In Lobby Game Start Logic Call Part!"));

	//EquipmentWorldSubSystem->NotifyGameStartInLobbyBroadCast();
}
