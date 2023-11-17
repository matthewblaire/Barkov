// (c) Matthew Blaire 2023


#include "InteractiveObjectBase.h"


#include "Components/SphereComponent.h"

AInteractiveObjectBase::AInteractiveObjectBase()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	
	
	AreaSphere = CreateDefaultSubobject<USphereComponent>("Trigger area");
	ObjectVisual  = CreateDefaultSubobject<UStaticMeshComponent>("Object visual");

	SetRootComponent(ObjectVisual);
	AreaSphere->SetupAttachment(ObjectVisual);
	AreaSphere->SetSphereRadius(InteractionRange);

	// put AreaSphere in the middle of the ObjectVisual
	if (ObjectVisual->GetStaticMesh())
	{
		AreaSphere->SetRelativeLocation(ObjectVisual->GetStaticMesh()->GetBounds().GetBox().GetCenter());
	}
	
	
}

void AInteractiveObjectBase::Interact(AActor* Interactor)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Cyan,  FString::Printf(TEXT("Interacted! %s"), *PromptText));	
}

void AInteractiveObjectBase::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority() == false)
	{
		
	}
	else
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	/// previously this ran on both the server and every client, but I found it pointless to
	/// burden the server with the extra load, when each interaction is already verified upon interaction
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	
}

#if WITH_EDITOR
void AInteractiveObjectBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AInteractiveObjectBase, InteractionRange))
	{
		AreaSphere->SetSphereRadius(InteractionRange);
		// put AreaSphere in the middle of the ObjectVisual
		if (ObjectVisual->GetStaticMesh())
		{
			AreaSphere->SetRelativeLocation(ObjectVisual->GetStaticMesh()->GetBounds().GetBox().GetCenter());
		}
	}
}
#endif
