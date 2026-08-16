#include "EquipmentActor/KeyPad_Related_Actors/AToggleSwitch.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "Components/CapsuleComponent.h"

AAToggleSwitch::AAToggleSwitch()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MDFinder_Body(TEXT("/Game/VRContent/Modeling/22_ToggleSwitch/ToggleSwitch_Base.ToggleSwitch_Base"));
	if (ActorBaseMesh)
		ActorBaseMesh->SetStaticMesh(MDFinder_Body.Object);

	SM_Switch = CreateDefaultSubobject<UStaticMeshComponent>("SwitchSMComp");
	if (SM_Switch)
	{
		SM_Switch->SetupAttachment(ActorBaseMesh);

		static ConstructorHelpers::FObjectFinder<UStaticMesh> MDFinder_Switch(TEXT("/Game/VRContent/Modeling/22_ToggleSwitch/ToggleSwitch_Switch.ToggleSwitch_Switch"));
		if (MDFinder_Switch.Succeeded())
			SM_Switch->SetStaticMesh(MDFinder_Switch.Object);
	}

	CL_Switch = CreateDefaultSubobject<UCapsuleComponent>("SwitchColComp");
	if (CL_Switch)
	{
		CL_Switch->SetupAttachment(SM_Switch);
		CL_Switch->SetRelativeLocation(FVector(0.f, 1.f, 2.6f));
		CL_Switch->SetRelativeRotation(FRotator(0.f, 0.f, 20.f));
		CL_Switch->SetCapsuleRadius(1.0f);
		CL_Switch->SetCapsuleHalfHeight(3.0f);
		CL_Switch->SetGenerateOverlapEvents(true);
		CL_Switch->SetHiddenInGame(false); // Debug
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_On(TEXT("/Game/VRContent/Sound/Wavs/ToggleSwitch/Sfx_ToggleSwitch_On.Sfx_ToggleSwitch_On"));
	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_Off(TEXT("/Game/VRContent/Sound/Wavs/ToggleSwitch/Sfx_ToggleSwitch_Off.Sfx_ToggleSwitch_Off"));
	if (SFXFinder_On.Succeeded() && SFXFinder_Off.Succeeded())
	{
		SFX_SwitchOn = SFXFinder_On.Object;
		SFX_SwitchOff = SFXFinder_Off.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MatFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MatFinder_Main.Object);
		SM_Switch->SetMaterial(0, MatFinder_Main.Object);
	}

	bIsSwitching = 0;

	// Disable navigation on all components so they do not affect the NavMesh
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

void AAToggleSwitch::BeginPlay()
{
	Super::BeginPlay();
	this->EquipmentRegistrable(this);
	CL_Switch->OnComponentBeginOverlap.AddDynamic(this, &AAToggleSwitch::OnSwitchOverlapBegin);

	EquipmentWorldSubSystem->FKeyPadToToggleSignature.BindUObject(this, &AAToggleSwitch::HandleToggleSwitch);
}

void AAToggleSwitch::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);
}

void AAToggleSwitch::HandleToggleSwitch(uint8 InMoveFlag)
{
	if (InMoveFlag) // Back Move(Switch On)
	{
		mSoundPlayer->PlaySoundEffect(this, SFX_SwitchOn, ActorBaseMesh->GetComponentLocation());
		SM_Switch->SetRelativeRotation(FRotator(0.f, 0.f, -35.f));
		CL_Switch->SetGenerateOverlapEvents(false);
		CL_Switch->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
	else // Front Move(Switch Off)
	{
		mSoundPlayer->PlaySoundEffect(this, SFX_SwitchOff, ActorBaseMesh->GetComponentLocation());
		SM_Switch->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		CL_Switch->SetGenerateOverlapEvents(true);
		CL_Switch->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		bIsSwitching = 0;
	}
}

void AAToggleSwitch::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}

void AAToggleSwitch::OnSwitchOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("HandIndex")) && !bIsSwitching)
	{
		UE_LOG(LogTemp, Log, TEXT("Toggle Switch By Hand Index"));
		bIsSwitching = 1;
		HandleToggleSwitch(bIsSwitching);
		EquipmentWorldSubSystem->NotifyKeyPadOperationBroadCast(bIsSwitching);
	}
}

