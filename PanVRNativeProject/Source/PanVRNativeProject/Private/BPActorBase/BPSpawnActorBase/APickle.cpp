// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/BPSpawnActorBase/APickle.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

AAPickle::AAPickle()
{
	SKM_CucumberBody = CreateDefaultSubobject<USkeletalMeshComponent>("Cum_SKMComp");
	if (SKM_CucumberBody)
	{
		this->SetRootComponent(SKM_CucumberBody);
		SKM_CucumberBody->SetDefaultAnimatingRigOverride(TSoftObjectPtr<UObject>(FSoftObjectPath(TEXT("/Game/VRContent/Modeling/Cucumber/Cucumber_CtrlRig.Cucumber_CtrlRig"))));
		SKM_CucumberBody->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		SKM_CucumberBody->SetSimulatePhysics(true);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SKMFinder_Body(TEXT("/Game/VRContent/Modeling/Cucumber/Cucumber_Rig.Cucumber_Rig"));
	if (SKMFinder_Body.Succeeded())
	{
		SKM_CucumberBody->SetSkeletalMesh(SKMFinder_Body.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Body(TEXT("/Game/VRContent/Material/SRS_Stage_Cucumber.SRS_Stage_Cucumber"));
	if (MatFinder_Body.Succeeded())
	{
		SKM_CucumberBody->SetMaterial(0, MatFinder_Body.Object);
	}

	CL_CucumberBody = CreateDefaultSubobject<UCapsuleComponent>("CumCollisionComp");
	if (CL_CucumberBody)
	{
		CL_CucumberBody->SetupAttachment(SKM_CucumberBody);
		CL_CucumberBody->SetCapsuleRadius(3.0f);
		CL_CucumberBody->SetCapsuleHalfHeight(7.5f);
		CL_CucumberBody->SetRelativeLocation(FVector(1.1745f, 0.f, 0.f));
	}

	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetupAttachment(SKM_CucumberBody);
		ActorBaseMesh->SetCollisionProfileName(FName(TEXT("NoCollision")));
	}

	if (GC)
	{
		GC->SetupAttachment(SKM_CucumberBody);
	}
}

void AAPickle::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().UnPauseTimer(DestroyTimer);
	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimer,
		this,
		&AAPickle::DestroySelfActor,
		5.0f,
		false
	);
}

void AAPickle::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, AVRHand* InGrabbingHand)
{
	GetWorld()->GetTimerManager().PauseTimer(DestroyTimer);
}

void AAPickle::OnDropped()
{
	GetWorld()->GetTimerManager().UnPauseTimer(DestroyTimer);
	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimer,
		this,
		&AAPickle::DestroySelfActor,
		5.0f,
		false
	);
}

void AAPickle::DestroySelfActor()
{
	this->Destroy();
	GetWorld()->GetTimerManager().PauseTimer(DestroyTimer);
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimer);
}
