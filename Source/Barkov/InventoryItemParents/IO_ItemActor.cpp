// (c) Matthew Blaire 2023


#include "IO_ItemActor.h"

#include "Components/SphereComponent.h"

void AIO_ItemActor::BeginPlay()
{
	Super::BeginPlay();
	SetupInteractiveObject_BeginPlay();
}

AIO_ItemActor::AIO_ItemActor()
{
	SetupInteractiveObject_Constructor();
}

void AIO_ItemActor::SetGlow(bool bShouldGlow)
{
	ObjectVisual->SetRenderCustomDepth(bShouldGlow);
}

void AIO_ItemActor::Interact(AActor* Interactor)
{
	UAC_Inventory* InteractorInventory;
	
	
	if (UActorComponent* InteractorInventory_ActorComponent = Interactor->GetComponentByClass(UAC_Inventory::StaticClass()))
	{
		InteractorInventory = Cast<UAC_Inventory>(InteractorInventory_ActorComponent);
		if (InteractorInventory)
		{
			// add self to inventory and remove self from world
			// on Interactor Inventory, request data from this inventory to start delegate process
			//

			BP_Interact(Interactor, InteractorInventory);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact failed. Interactor inventory not found"));
		return;
	}
}

void AIO_ItemActor::SetupInteractiveObject_Constructor()
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

void AIO_ItemActor::SetupInteractiveObject_BeginPlay()
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

void AIO_ItemActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
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
