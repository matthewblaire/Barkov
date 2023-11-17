// (c) Matthew Blaire 2023


#include "Door.h"

#include "Net/UnrealNetwork.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DoorHinge = CreateDefaultSubobject<USceneComponent>("Door Hinge");
	SetRootComponent(DoorHinge);
	ObjectVisual->SetupAttachment(DoorHinge);
	bIsOpen = false;
	YawTarget = 0;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

	PromptText = bIsOpen ? TEXT("Close") : TEXT("Open");
}

void ADoor::Interact(AActor* Interactor)
{
	Super::Interact(Interactor);
	OpenerLocation = Interactor->GetActorLocation();

	if (HasAuthority())
	{
		bIsOpen = !bIsOpen;
		PromptText = bIsOpen ? TEXT("Close") : TEXT("Open");
		StartAnimation();
	}
	
}



void ADoor::StartAnimation()
{
	FVector TowardsOpener = OpenerLocation - GetActorLocation();
	double DotProduct = FVector::DotProduct(GetActorRightVector(),TowardsOpener);
	YawTargetMultiplier = DotProduct <= 0 ? 1 : -1;
	
	YawTarget =  YawTargetMultiplier * (bIsOpen ? 100.0 : 0);
	SetActorTickEnabled(true);
}

void ADoor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	auto Rot = ObjectVisual->GetRelativeRotation();
	
	if (FMath::RoundToDouble(Rot.Yaw)!=FMath::RoundToDouble(YawTarget))
	{
		if (Rot.Yaw < YawTarget)
		{
			Rot.Yaw += DoorSpeed * DeltaSeconds;
			if (Rot.Yaw > YawTarget) Rot.Yaw = YawTarget;
		}
		else
		{
			Rot.Yaw -= DoorSpeed * DeltaSeconds;
			if (Rot.Yaw < YawTarget) Rot.Yaw = YawTarget;
		}
		
		ObjectVisual->SetRelativeRotation(Rot);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DONE"));
		SetActorTickEnabled(false);
	}
}

void ADoor::OnRep_bIsOpen()
{
	PromptText = bIsOpen ? TEXT("Close") : TEXT("Open"); // make sure prompt text is up to date
}

void ADoor::OnRep_YawTarget()
{
	SetActorTickEnabled(true); // start animation without performing any math because we already have a YawTarget
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoor, bIsOpen);
	DOREPLIFETIME(ADoor, YawTarget);
}

