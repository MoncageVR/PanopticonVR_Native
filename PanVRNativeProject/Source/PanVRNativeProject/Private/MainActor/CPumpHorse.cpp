#include "MainActor/CPumpHorse.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "AIController.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreCommon/PrisonerRelated/PrisonerCharacter.h"
#include "CoreCommon/PrisonerRelated/PrisonerController.h"

ACPumpHorse::ACPumpHorse()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SKMFinder_Horse(TEXT("/Game/VRContent/Modeling/17_PumpAndPumpHorse/PumpHorse/Final_PumpHorse_Mesh.Final_PumpHorse_Mesh"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));

	if (this->GetMesh())
	{
		this->GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -87.5f));
		this->GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		this->GetMesh()->SetCollisionProfileName(FName("NoCollision"));

		if (SKMFinder_Horse.Succeeded() && MatFinder_Main.Succeeded())
		{
			this->GetMesh()->SetSkeletalMesh(SKMFinder_Horse.Object);
			this->GetMesh()->SetMaterial(0, MatFinder_Main.Object);
		}
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ABPFinder_PumpHorse(TEXT("/Game/VRContent/Modeling/17_PumpAndPumpHorse/PumpHorse/ABP_PumpHorse.ABP_PumpHorse_C"));
	if (ABPFinder_PumpHorse.Succeeded())
	{
		this->GetMesh()->SetAnimClass(ABPFinder_PumpHorse.Class);
	}

	if (this->GetCharacterMovement())
	{
		this->GetCharacterMovement()->MaxStepHeight = 150.0f;
		this->GetCharacterMovement()->SetWalkableFloorAngle(60.0f);
		this->GetCharacterMovement()->GroundFriction = 0.0f;
		this->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	}

	CL_PumpHorseForSubdue = CreateDefaultSubobject<UBoxComponent>("SubdueColComp");
	if (CL_PumpHorseForSubdue)
	{
		CL_PumpHorseForSubdue->SetupAttachment(this->RootComponent);
		CL_PumpHorseForSubdue->SetRelativeLocation(FVector(0.f, 0.f, 27.1f));
		CL_PumpHorseForSubdue->SetBoxExtent(FVector(170.0f, 40.0f, 100.0f));
		CL_PumpHorseForSubdue->SetCollisionProfileName(FName("OverlapAll"));
		CL_PumpHorseForSubdue->SetGenerateOverlapEvents(true);
		CL_PumpHorseForSubdue->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CL_PumpHorseForSubdue->SetHiddenInGame(false); // Debug
	}
}

void ACPumpHorse::BeginPlay()
{
	Super::BeginPlay();

	CL_PumpHorseForSubdue->OnComponentBeginOverlap.AddDynamic(this, &ACPumpHorse::OnSubdueOverlapBegin);
	CL_PumpHorseForSubdue->OnComponentEndOverlap.AddDynamic(this, &ACPumpHorse::OnSubdueOverlapEnd);

	UVREquipmentWorldSubsystem* TempVREquipWorldSubSy = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	check(TempVREquipWorldSubSy);
	if (TempVREquipWorldSubSy)
	{
		TempVREquipWorldSubSy->FPumpHorseOneStepSignature.BindUObject(this, &ACPumpHorse::MoveOneStepByPump);
	}

	this->SpawnDefaultController();
	AICon = Cast<AAIController>(GetController());
	checkf(AICon, TEXT("HorseCharacter AI Controller Not Spawn!"));
	bIsCantMove = 0;
	SpawnPos = this->RootComponent->GetComponentLocation();
	UE_LOG(LogTemp, Log, TEXT("%s Spawn Success!"), *this->GetName());
}

void ACPumpHorse::MoveOneStepByPump()
{
	if (!bIsCantMove)
	{
		if (!(FMath::IsNearlyEqual(this->RootComponent->GetComponentLocation().X, -2061.0f)))
		{
			FVector TempTargetPos = (SpawnPos - FVector(80.0f, 0.f, 0.f));
			SpawnPos = FVector(TempTargetPos.X, 0.f, this->RootComponent->GetComponentLocation().Z);
			AICon->MoveToLocation(
				SpawnPos,
				1.0f,
				true
			);
		}
		else
		{
			bIsCantMove = 1;
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("HorseCharacter Can't Move forward!!!"));
	}
}

void ACPumpHorse::ActuallySubduePrisoner(APrisonerController* Prisoner)
{
	if (!Prisoner) return;

	TArray<uint8> GivenUpperStates = { 1 };
	TArray<uint8> GivenLowerStates = { 1 };
	Prisoner->State_based_ExecutionTasks_GiventoSomeone(GivenUpperStates, GivenLowerStates);
}

void ACPumpHorse::HandleMoveEndAndDestroySelf()
{
	if (AICon)
	{
		AICon->ReceiveMoveCompleted.AddDynamic(this, &ACPumpHorse::MoveEndCompleted);
		//EPathFollowingRequestResult
		AICon->MoveToLocation(
			FVector(-300.0f, 0.0f, 151.0f),
			1.0f,
			true
		);
	}
}

void ACPumpHorse::MoveEndCompleted(FAIRequestID InRequestID, EPathFollowingResult::Type InResult)
{
	if (InResult == EPathFollowingResult::Success)
	{
		OverlapPrisonerConArrs.Empty();
		SpawnPos = this->RootComponent->GetComponentLocation();
		bIsCantMove = 0;
		UE_LOG(LogTemp, Log, TEXT("HorseCharacter Destroy Success!!"));
		AICon->ReceiveMoveCompleted.RemoveDynamic(this, &ACPumpHorse::MoveEndCompleted);
		this->Destroy();
	}
}

void ACPumpHorse::OnSubdueOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName(TEXT("PrisonerCharacter"))))
	{
		APrisonerCharacter* TempCha = Cast<APrisonerCharacter>(OtherActor);
		check(TempCha);
		APrisonerController* TempChaCon = Cast<APrisonerController>(TempCha->GetController());
		check(TempChaCon);
		OverlapPrisonerConArrs.AddUnique(TempChaCon);

		TempChaCon->FOnPrisonerLowerStateChangedSignature.AddDynamic(this, &ACPumpHorse::OnOverlappingPrisonerStateChanged);

		if (TempChaCon->GetCurrLowerState() == 12)
		{
			ActuallySubduePrisoner(TempChaCon);
		}
	}
}

void ACPumpHorse::OnSubdueOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName(TEXT("PrisonerCharacter"))))
	{
		APrisonerCharacter* TempCha = Cast<APrisonerCharacter>(OtherActor);
		check(TempCha);
		APrisonerController* TempChaCon = Cast<APrisonerController>(TempCha->GetController());
		check(TempChaCon);

		TempChaCon->FOnPrisonerLowerStateChangedSignature.RemoveDynamic(this, &ACPumpHorse::OnOverlappingPrisonerStateChanged);

		OverlapPrisonerConArrs.RemoveSingleSwap(TempChaCon);
	}
}

void ACPumpHorse::OnOverlappingPrisonerStateChanged(APrisonerController* Prisoner, uint8 NewLowerState)
{
	if (NewLowerState == 4 && OverlapPrisonerConArrs.Contains(Prisoner))
	{
		ActuallySubduePrisoner(Prisoner);
	}
}
