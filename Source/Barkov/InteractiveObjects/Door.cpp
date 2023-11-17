// (c) Matthew Blaire 2023


#include "Door.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"


void ADoor::SetupInteractiveObject_Constructor()
{
	AreaSphere = CreateDefaultSubobject<USphereComponent>("Trigger area");
	
	ObjectVisual  = CreateDefaultSubobject<UStaticMeshComponent>("Object visual");
	
	ObjectVisual->CustomDepthStencilValue = 2;
	
	AreaSphere->SetupAttachment(ObjectVisual);
	AreaSphere->SetSphereRadius(InteractionRange);
	
	// put AreaSphere in the middle of the ObjectVisual
	if (ObjectVisual->GetStaticMesh())
	{
		AreaSphere->SetRelativeLocation(ObjectVisual->GetStaticMesh()->GetBounds().GetBox().GetCenter());
	}
	
}

void ADoor::SetupInteractiveObject_BeginPlay()
{

	if (HasAuthority() == false || IsNetMode(NM_Standalone))
	{
		
	}
	else
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	/// this runs on both the server and every client, but I find it pointless to
	/// burden the server with the extra load, when each interaction is already verified upon interaction
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	
}

ADoor::ADoor()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	
	DoorHinge = CreateDefaultSubobject<USceneComponent>("Door Hinge");
	SetRootComponent(DoorHinge);
	
	SetupInteractiveObject_Constructor();
	
	ObjectVisual->SetupAttachment(DoorHinge);
	bIsOpen = false;
	YawTarget = 0;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

	PromptText = FText::FromString(bIsOpen ? TEXT("Close") : TEXT("Open"));
	

	SetupInteractiveObject_BeginPlay();
	
}

void ADoor::Interact(AActor* Interactor)
{
	OpenerLocation = Interactor->GetActorLocation();

	if (HasAuthority())
	{
		bIsOpen = !bIsOpen;
		PromptText = FText::FromString(bIsOpen ? TEXT("Close") : TEXT("Open"));
		StartAnimation();
	}
	
}



void ADoor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, InteractionRange))
	{
		AreaSphere->SetSphereRadius(InteractionRange);
		// put AreaSphere in the middle of the ObjectVisual
		if (ObjectVisual->GetStaticMesh())
		{
			AreaSphere->SetRelativeLocation(ObjectVisual->GetStaticMesh()->GetBounds().GetBox().GetCenter());
		}
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
	PromptText = FText::FromString(bIsOpen ? TEXT("Close") : TEXT("Open")); // make sure prompt text is up to date
}

void ADoor::OnRep_YawTarget()
{
	SetActorTickEnabled(true); // start animation without performing any math because we already have a YawTarget
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bIsOpen);
	DOREPLIFETIME(ThisClass, YawTarget);
}

void ADoor::SetGlow(bool bShouldGlow)
{
	ObjectVisual->SetRenderCustomDepth(bShouldGlow);
}

