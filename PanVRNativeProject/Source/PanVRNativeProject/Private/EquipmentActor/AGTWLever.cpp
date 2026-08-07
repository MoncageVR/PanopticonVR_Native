#include "EquipmentActor/AGTWLever.h"
#include "Components/CapsuleComponent.h"
#include "CoreCommon/Component/UGrabComp.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"

AAGTWLever::AAGTWLever()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setting Static Mesh Lever Body
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_GTWBody(TEXT("/Game/VRContent/Modeling/12_GoToWorkLever/SM_GTW_Lever_Body.SM_GTW_Lever_Body"));
	if (ModelingFinder_GTWBody.Succeeded())
		ActorBaseMesh->SetStaticMesh(ModelingFinder_GTWBody.Object);

	// Setting Handle Root
	HandleRoot = CreateDefaultSubobject<USceneComponent>("SceneComp");
	HandleRoot->SetupAttachment(ActorBaseMesh);

	// Setting Static Mesh Lever Handle
	SM_Handle = CreateDefaultSubobject<UStaticMeshComponent>("SM_Handle");
	SM_Handle->SetupAttachment(HandleRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_GTWHandle(TEXT("/Game/VRContent/Modeling/12_GoToWorkLever/SM_GTW_Lever_Handle.SM_GTW_Lever_Handle"));
	if (ModelingFinder_GTWHandle.Succeeded())
	{
		SM_Handle->SetStaticMesh(ModelingFinder_GTWHandle.Object);
	}

	// Setting Handle Under Movement Standard Root
	LeverMovementStandard = CreateDefaultSubobject<USceneComponent>("MoveStandard");
	LeverMovementStandard->SetupAttachment(SM_Handle);
	LeverMovementStandard->SetRelativeLocation(FVector(-6.25f, 0.0f, 22.5f));

	// Setting Handle Collision Component
	HandleCapsule = CreateDefaultSubobject<UCapsuleComponent>("CL_Handle");
	HandleCapsule->SetupAttachment(SM_Handle);
	HandleCapsule->SetRelativeLocation(FVector(-6.25f, 0.0f, 22.5f));
	HandleCapsule->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	HandleCapsule->SetCapsuleRadius(2.7f, true);
	HandleCapsule->SetCapsuleHalfHeight(10.0f);
	HandleCapsule->SetHiddenInGame(false); // Debug

	// Setting Various Component Pos And Rot
	GC->SetupAttachment(SM_Handle);
	GC->EEGrabType = EGrabType::HandToObj;

	HandleCapsule->OnComponentBeginOverlap.AddDynamic(this, &AAGTWLever::OverlapCapsuleBegin);
	HandleCapsule->OnComponentEndOverlap.AddDynamic(this, &AAGTWLever::OverlapCapsuleEnd);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
		SM_Handle->SetMaterial(0, MaterialFinder_Main.Object);
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

void AAGTWLever::BeginPlay()
{
	Super::BeginPlay();

	EquipmentRegistrable(this);
}

void AAGTWLever::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}

void AAGTWLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAGTWLever::OverlapCapsuleBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("HandCLBox")))
	{
		bIsHanding = true;
	}
}

void AAGTWLever::OverlapCapsuleEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("HandCLBox")))
	{
		bIsHanding = false;
	}
}

void AAGTWLever::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{
	OperatingMCRef = &InMCRef;
	GC->SetRelativeLocation(FVector(-5.0f, 2.0f, 16.0f));
	GC->SetRelativeRotation(FRotator(180.0f, 0.0f, 90.0f));

	// Processing For Already Lever Operation
	if (CheckGTWLeverOpationAngle())
	{
		return;
	}

	if (bIsHanding)
	{
		GetWorldTimerManager().SetTimer(
			LeverOperateTimer,
			this,
			&AAGTWLever::OperateLever,
			0.01f,
			true
		);
	}
	else
	{
		return;
	}

	return;
}

void AAGTWLever::OnDropped()
{
	GetWorldTimerManager().PauseTimer(LeverOperateTimer);
	OperatingMCRef = nullptr;

	if (CheckGTWLeverOpationAngle())
	{
		UE_LOG(LogTemp, Log, TEXT("On Dropped Angle is IN Operate Angle!"));
		SM_Handle->SetRelativeRotation(FRotator(0.0f, 180.0f, 89.0f));
		HandleCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetWorldTimerManager().SetTimer(
			CountGameStartTimer,
			this,
			&AAGTWLever::CountForGameStart,
			5.0f,
			false
		);

	}
	else
	{
		SM_Handle->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
}

// Execution Function In Lever Movement Timer
void AAGTWLever::OperateLever()
{
	FTransform Between_MCAndStandard_MakeRelativeTransform = OperatingMCRef->GetComponentTransform().GetRelativeTransform(LeverMovementStandard->GetRelativeTransform());

	FVector LeverMovementDirection = LeverMovementStandard->GetRelativeLocation() + Between_MCAndStandard_MakeRelativeTransform.GetLocation();

	float TargetRoll = FMath::Clamp((SM_Handle->GetRelativeRotation().Roll - LeverMovementDirection.X), -50.0f, 89.0f);

	SM_Handle->SetRelativeRotation(FRotator(0.0f, 180.0f, TargetRoll));
}

bool AAGTWLever::CheckGTWLeverOpationAngle()
{
	bool TempResult = FMath::IsWithinInclusive(SM_Handle->GetRelativeRotation().Roll, 80.0f, 89.0f);
	return TempResult;
}

void AAGTWLever::CountForGameStart()
{
	if (EquipmentWorldSubSystem)
		EquipmentWorldSubSystem->NotifyGameStartBroadCast();

	GetWorldTimerManager().PauseTimer(CountGameStartTimer);
}
