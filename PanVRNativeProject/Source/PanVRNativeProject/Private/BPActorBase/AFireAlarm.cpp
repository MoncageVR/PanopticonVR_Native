// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/AFireAlarm.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AAFireAlarm::AAFireAlarm()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_MainBody(TEXT("/Game/VRContent/Modeling/FireAlarm/FireAlam_Body.FireAlam_Body"));
	if (ModelingFinder_MainBody.Succeeded() && ActorBaseMesh)
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_MainBody.Object);
	}

	FAFirstArm = CreateDefaultSubobject<UStaticMeshComponent>("SM_Arm1");
	FASecondArm = CreateDefaultSubobject<UStaticMeshComponent>("SM_Arm2");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Arm(TEXT("/Game/VRContent/Modeling/FireAlarm/FireAlam_Arm.FireAlam_Arm"));

	if (FAFirstArm && FASecondArm)
	{
		FAFirstArm->SetupAttachment(ActorBaseMesh);
		FAFirstArm->SetRelativeLocation(FVector(0.f, -23.f, -22.f));
		FASecondArm->SetupAttachment(FAFirstArm);
		FASecondArm->SetRelativeLocation(FVector(-23.f, -15.f, -14.7f));
		FASecondArm->SetRelativeRotation(FRotator(0.f, -57.8f, 0.f));

		if (ModelingFinder_Arm.Succeeded())
		{
			FAFirstArm->SetStaticMesh(ModelingFinder_Arm.Object);
			FASecondArm->SetStaticMesh(ModelingFinder_Arm.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
		FAFirstArm->SetMaterial(0, MaterialFinder_Main.Object);
		FASecondArm->SetMaterial(0, MaterialFinder_Main.Object);
	}

	CLFAFireCheck = CreateDefaultSubobject<UBoxComponent>("FireCheckCollision");
	if (CLFAFireCheck)
	{
		CLFAFireCheck->SetupAttachment(ActorBaseMesh);
		CLFAFireCheck->SetRelativeLocation(FVector(0.f, 21.f, -7.55f));
		CLFAFireCheck->SetBoxExtent(FVector(5.f, 4.f, 5.f));
	}

	CLFAFireCheck->OnComponentBeginOverlap.AddDynamic(this, &AAFireAlarm::FireCheckColOverlapBegin);
	CLFAFireCheck->OnComponentEndOverlap.AddDynamic(this, &AAFireAlarm::FireCheckColOverlapEnd);

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_SFXFireAlarm(TEXT("/Game/VRContent/Sound/Wavs/FireAlarm_Sprinkler_/sfx_lighter_alarm.sfx_lighter_alarm"));
	if (SoundFinder_SFXFireAlarm.Succeeded())
	{
		SFXFireAlarm = SoundFinder_SFXFireAlarm.Object;
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

void AAFireAlarm::BeginPlay()
{
	Super::BeginPlay();
}

void AAFireAlarm::Tick(float DeltatTimes)
{
	Super::Tick(DeltatTimes);
}

void AAFireAlarm::FireCheckColOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("Fire")))
	{
		GetWorldTimerManager().SetTimer(
			SprinklerOperateTimer,
			this,
			&AAFireAlarm::SprinklerOperation,
			4.0f,
			false
		);
	}
}

void AAFireAlarm::FireCheckColOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName("Fire")))
	{
		GetWorldTimerManager().PauseTimer(SprinklerOperateTimer);
		GetWorldTimerManager().ClearTimer(SprinklerOperateTimer);
	}
}

void AAFireAlarm::SprinklerOperation()
{
	UGameplayStatics::PlaySoundAtLocation(this, SFXFireAlarm, CLFAFireCheck->GetComponentLocation());

	/*
	Sprinkler Logic Execution Part
	*/

	SprinklerCoolDown();
}

void AAFireAlarm::SprinklerCoolDown()
{
	CLFAFireCheck->SetGenerateOverlapEvents(false);

	GetWorldTimerManager().SetTimer(
		SprinklerCooldownTimer,
		this,
		&AAFireAlarm::SprinklerCoolDownEnd,
		10.0f,
		false
	);
}

void AAFireAlarm::SprinklerCoolDownEnd()
{
	CLFAFireCheck->SetGenerateOverlapEvents(true);

	GetWorldTimerManager().ClearTimer(SprinklerCooldownTimer);
	GetWorldTimerManager().PauseTimer(SprinklerCooldownTimer);
}
