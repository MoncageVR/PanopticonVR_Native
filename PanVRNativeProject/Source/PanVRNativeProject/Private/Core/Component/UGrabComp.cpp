// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Component/UGrabComp.h"
#include "MotionControllerComponent.h"
#include "Core/Debug/FDebugLib.h"
#include "Components/SkeletalMeshComponent.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Interface/IGrabInterface.h"
#include "VRPawn/Hand/VRHand.h"

UUGrabComp::UUGrabComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UUGrabComp::BeginPlay()
{
	Super::BeginPlay();
	SetShouldSimulateOnDrop();
}

// If the Top Parent Component(PrimitiveComp) that GC Comp Attached is Physics Simulating, Values that bSimulateDrop is True And Collision Profile Name PhysicsActor Setting
void UUGrabComp::SetShouldSimulateOnDrop()
{
	USceneComponent* TopParentComp = GetAttachParent();

	if (UPrimitiveComponent* TopPrimitiveComp = Cast<UPrimitiveComponent>(TopParentComp))
	{
		if (TopPrimitiveComp)
		{
			if (TopPrimitiveComp->IsSimulatingPhysics())
				bSimulateOnDrop = true;

			TopPrimitiveComp->SetCollisionProfileName(FName("PhysicsActor"), true);
		}
	}
}

// Grab Component Try Grab Function
void UUGrabComp::GCTryGrab(UMotionControllerComponent* InMC, USkeletalMeshComponent* InSMComp, class AVRHand* InGrabbingHand)
{
	MCRef = InMC;
	MHand = InGrabbingHand;

	if (MCRef)
	{
		switch (EEGrabType)
		{
		case EGrabType::Free:
			SetPrimitiveCompPhysics(false);
			if (GetOwner()->GetClass()->ImplementsInterface(UIGrabInterface::StaticClass()))
			{
				CurrentIGrabInterface.SetObject(GetOwner());
				CurrentIGrabInterface.SetInterface(Cast<IIGrabInterface>(GetOwner()));
				if (CurrentIGrabInterface)
				{
					CurrentIGrabInterface->OnGrabbed(*MCRef, InSMComp->GetComponentLocation(), MHand);
				}
			}
			AttachGCParentToMotionController(MCRef, GetAttachParent()); // GC Parent->MC
			bIsHeld = true;
			break;
		case EGrabType::ObjToHand:
			SetPrimitiveCompPhysics(false);
			AttachGCParentToMotionController(MCRef, GetAttachParent()); // GC Parent->MC
			bIsHeld = true;
			AttachGCParentToHand(InMC); // GC Parent->HandMesh
			if (GetOwner()->GetClass()->ImplementsInterface(UIGrabInterface::StaticClass()))
			{
				CurrentIGrabInterface.SetObject(GetOwner());
				CurrentIGrabInterface.SetInterface(Cast<IIGrabInterface>(GetOwner()));
				if (CurrentIGrabInterface)
				{
					CurrentIGrabInterface->OnGrabbed(*MCRef, InSMComp->GetComponentLocation(), MHand);
				}
			}
			break;
		case EGrabType::HandToObj:
			GrabbedBySkeletalMesh = InSMComp;

			if (GetOwner()->GetClass()->ImplementsInterface(UIGrabInterface::StaticClass()))
			{
				CurrentIGrabInterface.SetObject(GetOwner());
				CurrentIGrabInterface.SetInterface(Cast<IIGrabInterface>(GetOwner()));
			}

			if (GrabbedBySkeletalMesh) // Check Skeletal Mesh Reference Initialize Success
			{
				if (InMC->ComponentHasTag(FName("RightMC"))) // Right Hand Grab Case
				{
					if (CurrentIGrabInterface)
					{
						CurrentIGrabInterface->OnGrabbed(*MCRef, InSMComp->GetComponentLocation(), MHand);
					}
					AttachHandToGCParent(GrabbedBySkeletalMesh, GetAttachParent());

				}
				else if (InMC->ComponentHasTag(FName("LeftMC"))) // Left Hand Grab Case
				{
					if (CurrentIGrabInterface)
					{
						CurrentIGrabInterface->OnGrabbed(*MCRef, InSMComp->GetComponentLocation(), MHand);
					}
					AttachHandToGCParent(GrabbedBySkeletalMesh, GetAttachParent());
				}
			}
			break;
		case EGrabType::SpawnObj:
			bIsHeld = true;
			if (GetOwner()->GetClass()->ImplementsInterface(UIGrabInterface::StaticClass()))
			{
				CurrentIGrabInterface.SetObject(GetOwner());
				CurrentIGrabInterface.SetInterface(Cast<IIGrabInterface>(GetOwner()));
				if (CurrentIGrabInterface)
				{
					CurrentIGrabInterface->OnGrabbed(*MCRef, InSMComp->GetComponentLocation(), MHand);
				}
			}
			break;
		case EGrabType::AutomaticObj:
			bIsHeld = true;
			if (GetOwner()->GetClass()->ImplementsInterface(UIGrabInterface::StaticClass()))
			{
				CurrentIGrabInterface.SetObject(GetOwner());
				CurrentIGrabInterface.SetInterface(Cast<IIGrabInterface>(GetOwner()));
				if (CurrentIGrabInterface)
				{
					CurrentIGrabInterface->OnGrabbed(*MCRef, InSMComp->GetComponentLocation(), MHand);
				}
			}
			break;
		default:
			break;
		}
	}
}

// Grab Component Try Release Function
bool UUGrabComp::GCTryRelease()
{
	switch (EEGrabType)
	{
	case EGrabType::Free:
		if (bIsHeld)
		{
			GetAttachParent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			CurrentIGrabInterface->OnDropped();
			CurrentIGrabInterface = nullptr;
			GrabbedBySkeletalMesh = nullptr;
			MCRef = nullptr;
			bIsHeld = false;
		}
		break;
	case EGrabType::ObjToHand:
		if (bIsHeld)
		{
			GetAttachParent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			CurrentIGrabInterface->OnDropped();
			CurrentIGrabInterface = nullptr;
			GrabbedBySkeletalMesh = nullptr;
			MCRef = nullptr;
			bIsHeld = false;
		}
		break;
	case EGrabType::HandToObj:
		if (bIsHeld)
		{
			GrabbedBySkeletalMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			CurrentIGrabInterface->OnDropped();
			CurrentIGrabInterface = nullptr;
			GrabbedBySkeletalMesh = nullptr;
			MCRef = nullptr;
			bIsHeld = false;
		}
		break;
	case EGrabType::SpawnObj:
		if (bIsHeld)
		{
			CurrentIGrabInterface->OnDropped();
			CurrentIGrabInterface = nullptr;
			GrabbedBySkeletalMesh = nullptr;
			MCRef = nullptr;
			bIsHeld = false;
		}
		break;
	case EGrabType::AutomaticObj:
		if (bIsHeld)
		{
			CurrentIGrabInterface->OnDropped();
			CurrentIGrabInterface = nullptr;
			GrabbedBySkeletalMesh = nullptr;
			MCRef = nullptr;
			bIsHeld = false;
		}
		break;
	default:
		break;
	}

	return false;
}



void UUGrabComp::SetPrimitiveCompPhysics(bool InSimulate)
{
	USceneComponent* TopParentComp = GetAttachParent();

	if (UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(TopParentComp))
	{
		RootComp->SetSimulatePhysics(InSimulate);
	}
	else
	{
		DEBUG_PRINTSCREEN(10.0f, FColor::Yellow, TEXT("LEEEEEE"));
	}
}

EControllerHand UUGrabComp::GetHeldByHand()
{
	if (MCRef->MotionSource == FName("Left"))
	{
		return EControllerHand::Left;
	}
	if(MCRef->MotionSource == FName("Right"))
	{
		return EControllerHand::Right;
	}
	else
	{
		return EControllerHand();
	}
}

void UUGrabComp::AttachGCParentToMotionController(UMotionControllerComponent* InMC, USceneComponent* InTargetObj)
{
	if (InMC && InTargetObj)
	{
		bool AttachResult = InTargetObj->AttachToComponent(InMC, FAttachmentTransformRules::KeepWorldTransform);

		if (AttachResult)
		{
			return;
		}
		else // Debug
		{
			FString a = TEXT("Attaching MotionController To GrabComp Failed-Object, Not Grabbed!");
			DEBUG_PRINTSCREEN(100.0f, FColor::Red, a);
			return;
		}
	}
}

void UUGrabComp::AttachHandToGCParent(USkeletalMeshComponent* InSMComp, USceneComponent* InAttachParent)
{
	bool TempAttachResult = GrabbedBySkeletalMesh->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	if (TempAttachResult)
		bIsHeld = true;

	return;
}

void UUGrabComp::AttachGCParentToHand(UMotionControllerComponent* InMCRef)
{
	UMotionControllerComponent* TempMCRef = InMCRef;

	//FRotator SelfRelativeRot = GetRelativeRotation();
	FRotator SelfRelativeInvertRot = GetRelativeRotation().GetInverse();
	GetAttachParent()->SetRelativeRotation(SelfRelativeInvertRot, false, nullptr, ETeleportType::TeleportPhysics);

	FVector FinalLocation = TempMCRef->GetComponentLocation() + ((this->GetComponentLocation() - GetAttachParent()->GetComponentLocation()) * -1.0f);

	GetAttachParent()->SetWorldLocation(FinalLocation, false, nullptr, ETeleportType::TeleportPhysics);

	return;
}

const UMotionControllerComponent* UUGrabComp::GetMCRef()
{
	return MCRef;
}