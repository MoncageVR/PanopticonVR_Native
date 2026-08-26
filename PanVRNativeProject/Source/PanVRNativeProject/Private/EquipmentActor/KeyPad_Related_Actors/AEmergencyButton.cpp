#include "EquipmentActor/KeyPad_Related_Actors/AEmergencyButton.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreObj/Manager/GameInstanceSubSystem/PrisonerManagerSubsystem.h"
#include "CoreCommon/PrisonerRelated/PrisonerController.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"

AAEmergencyButton::AAEmergencyButton()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ButtonBody(TEXT("/Game/VRContent/Modeling/21_StopButton(EmergencyButton)/SM_ButtonBody.SM_ButtonBody"));
	if (ActorBaseMesh && ModelingFinder_ButtonBody.Succeeded())
	{
		ActorBaseMesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
		ActorBaseMesh->SetStaticMesh(ModelingFinder_ButtonBody.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ButtonHead(TEXT("/Game/VRContent/Modeling/21_StopButton(EmergencyButton)/SM_ButtonHead.SM_ButtonHead"));
	EMButton = CreateDefaultSubobject<UStaticMeshComponent>("SM_Button");
	if (EMButton && ModelingFinder_ButtonHead.Succeeded())
	{
		EMButton->SetupAttachment(ActorBaseMesh);
		EMButton->SetStaticMesh(ModelingFinder_ButtonHead.Object);
	}

	CLButton = CreateDefaultSubobject<UBoxComponent>("Col_Button");
	if (CLButton)
	{
		CLButton->SetupAttachment(EMButton);
		CLButton->SetRelativeLocation(FVector(0.f, 0.f, 17.75f));
		CLButton->SetBoxExtent(FVector(5.5f, 5.5f, 1.7f));
		CLButton->SetGenerateOverlapEvents(true);
	}

	SCButtonMoveRail = CreateDefaultSubobject<USplineComponent>("Button_SplineComp");
	if (SCButtonMoveRail)
	{
		SCButtonMoveRail->SetupAttachment(ActorBaseMesh);

		SCButtonMoveRail->SetLocationAtSplinePoint(0, FVector(0.f, 0.f, 0.f), ESplineCoordinateSpace::Local);
		SCButtonMoveRail->SetLocationAtSplinePoint(1, FVector(0.f, 0.f, -4.f), ESplineCoordinateSpace::Local);

		SCButtonMoveRail->SetTangentAtSplinePoint(0, FVector(0.f, 0.f, 0.f), ESplineCoordinateSpace::Local);
		SCButtonMoveRail->SetTangentAtSplinePoint(1, FVector(0.f, 0.f, 0.f), ESplineCoordinateSpace::Local);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MatFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MatFinder_Main.Object);
		EMButton->SetMaterial(0, MatFinder_Main.Object);
	}

	CLButton->OnComponentBeginOverlap.AddDynamic(this, &AAEmergencyButton::EMButtonColOverlapBegin);

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_Success(TEXT("/Game/VRContent/Sound/Wavs/EmergencyButton/sfx_keypad_success.sfx_keypad_success"));
	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_Fail(TEXT("/Game/VRContent/Sound/Wavs/EmergencyButton/sfx_keypad_fail.sfx_keypad_fail"));
	if (SFXFinder_Success.Succeeded() && SFXFinder_Fail.Succeeded())
	{
		ButtonSuccessSFX = SFXFinder_Success.Object;
		ButtonFailSFX = SFXFinder_Fail.Object;
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

void AAEmergencyButton::BeginPlay()
{
	Super::BeginPlay();

	StopButtonFinalOutputIntArrays.Empty();
	StopButtonFinalOutputIntArrays.Reserve(4);

	EquipmentWorldSubSystem->FInputValuePassingByKeyPadSignature.BindUObject(this, &AAEmergencyButton::HandleThisReceiveByKeyPad);
}

void AAEmergencyButton::EMButtonColOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("HandIndex")) && !bIsHitting)
	{
		UPrisonerManagerSubsystem* PrisonerMgrSubSy = (GetWorld()->GetGameInstance()->GetSubsystem<UPrisonerManagerSubsystem>());
		checkf(PrisonerMgrSubSy, TEXT("UPrisonerManagerSubSystem Not Valid"));

		EMButton->SetRelativeLocation(SCButtonMoveRail->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local));
		bIsHitting = 1;

		CLButton->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (TryGetSubdueRoomNumber())
		{
			UE_LOG(LogTemp, Log, TEXT("%d Prisoner Subdue Play!"), FinalRoomNum);

			HVRSoundPlayer::PlaySoundEffect(this, ButtonSuccessSFX, ActorBaseMesh->GetComponentLocation());
			ClearStopButtonOutputArray();
			CoolDownOperation(10.0f);

			TArray<uint8> GivenUpperStates = { 1 };
			TArray<uint8> GivenLowerStates = { 1 };

			PrisonerMgrSubSy->GetAllPrisonerControllerArr()[FinalRoomNum]->State_based_ExecutionTasks_GiventoSomeone(GivenUpperStates, GivenLowerStates);

			FinalRoomNum = 0;
		}
		else
		{
			HVRSoundPlayer::PlaySoundEffect(this, ButtonFailSFX, ActorBaseMesh->GetComponentLocation());
			ClearStopButtonOutputArray();
			CoolDownOperation(1.0f);
			FinalRoomNum = 0;
		}
	}
}

void AAEmergencyButton::OperateAfterReturn()
{
	EMButton->SetRelativeLocation(SCButtonMoveRail->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local));

	CLButton->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	bIsHitting = 0;
}

void AAEmergencyButton::HandleThisReceiveByKeyPad(bool bIsFlag, uint32 InputIndex)
{
	if (bIsFlag)
	{
		UE_LOG(LogTemp, Log, TEXT("AEmergencyButton In Text Array Clear"));
		ClearStopButtonOutputArray();
		CLButton->SetHiddenInGame(true); // Debug
	}
	else
	{
		if (InputIndex == 10)
			StopButtonFinalOutputIntArrays.Add(0);
		else
			StopButtonFinalOutputIntArrays.Add(InputIndex);

		if (StopButtonFinalOutputIntArrays.Num() >= 4)
		{
			//CLButton->SetGenerateOverlapEvents(true);
			CLButton->SetHiddenInGame(false); // Debug
		}
	}
}

void AAEmergencyButton::ClearStopButtonOutputArray()
{
	StopButtonFinalOutputIntArrays.Empty(); // EmergencyButton Output Array Clear
	EquipmentWorldSubSystem->NotifyKeyPadArrClearBroadCast(); // KeyPad Out Array Clear
}

void AAEmergencyButton::CoolDownOperation(float CoolDownTime)
{
	GetWorldTimerManager().SetTimer(
		ReturnTimer,
		this,
		&AAEmergencyButton::OperateAfterReturn,
		CoolDownTime,
		false
	);
}

bool AAEmergencyButton::TryGetSubdueRoomNumber()
{
	if (StopButtonFinalOutputIntArrays.Num() != 4)
		return false;

	FinalRoomNum = 0;
	for (uint32 digit : StopButtonFinalOutputIntArrays)
	{
		if (digit > 9) return false;
		FinalRoomNum = (FinalRoomNum * 10) + (int32)digit;
	}

	if (FinalRoomNum < 1 || FinalRoomNum > 24)
		return false;

	//UE_LOG(LogTemp, Log, TEXT("FinalRoomNum : %d"), FinalRoomNum);
	//UE_LOG(LogTemp, Log, TEXT("SB Subdue PrisonerNum : %d"), FinalRoomNum - 1);

	FinalRoomNum -= 1;

	return true;
}
