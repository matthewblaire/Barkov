// (c) Matthew Blaire 2023


#include "InteractionComponent.h"
#include "Barkov/Character/BarkovCharacter.h"
#include "Barkov/Interfaces/I_InteractiveObject.h"
#include "Barkov/UserInterface/InteractWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"


UInteractionComponent::UInteractionComponent():
Character(nullptr),
InteractWidget(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::UpdateInteractiveObjectCount(bool bIncrement)
{
	if (!Character->IsLocallyControlled()) return;
	
	switch(bIncrement)
	{
	case false:
		InteractiveObjectsNearby--;
		break;
	case true:
		InteractiveObjectsNearby++;
		break;
	}
	
	if (InteractiveObjectsNearby > 0 && !IsComponentTickEnabled())
	{
		SetComponentTickEnabled(true);
		UE_LOG(LogTemp, Warning, TEXT("YES Interact"));
	}
	else if (InteractiveObjectsNearby <= 0)
	{
		SetComponentTickEnabled(false);
		InteractTarget = nullptr;
		InteractTargetUpdated.Broadcast(InteractTarget);
		UE_LOG(LogTemp, Warning, TEXT("NO Interact"));
		InteractiveObjectsNearby = 0; // just in case it's below 0 lol
		
	}
}



void UInteractionComponent::HandleInteractiveObjectCheck()
{
	
	FHitResult Hit;
	FVector TraceStart = Character->GetCam()->GetComponentLocation();
	FVector TraceEnd = Character->GetCam()->GetComponentLocation() + Character->GetCam()->GetForwardVector() * 500;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params);
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		AActor* ObjectActor = Hit.GetActor();
		II_InteractiveObject* InteractiveObject = Cast<II_InteractiveObject>(ObjectActor);
		
		if (InteractiveObject)
		{ /* player is looking at an interactive object */
			
			// update interact target
			InteractTarget = ObjectActor;
			// broadcast update interact target delegate
			InteractTargetUpdated.Broadcast(InteractTarget);

			// make interact target glow
			InteractiveObject->SetGlow(true);
			
		}
		else
		{ /* player is looking at something, but it's not interactive */
			
			// before changing target, turn off the glow of the last target
			if (II_InteractiveObject* IOCast = Cast<II_InteractiveObject>(InteractTarget))
			{
				IOCast->SetGlow(false);
			}
			InteractTarget = nullptr;
			InteractTargetUpdated.Broadcast(InteractTarget);			
		}
	}
	else
	{ /* player is not looking at anything */
	
		// before changing target, turn off the glow of the last target
		if (II_InteractiveObject* IOCast = Cast<II_InteractiveObject>(InteractTarget))
		{
			IOCast->SetGlow(false);
		}
		if (InteractTarget != nullptr)
		{
			SetInteractTarget(nullptr);
		}
	}

	//draw debug line that matches the line trace from earlier
	if (bDrawDebugLine)
	{
		DrawDebugLine(GetWorld(), Character->GetActorLocation(), TraceEnd, FColor::Red, false, -1.f, 0, 5.f);
	}
}

void UInteractionComponent::OnRegister()
{
	Super::OnRegister();
	
	Character = Cast<ABarkovCharacter>(GetOwner()); // set up reference to owner
	Character->OnPossession.AddDynamic(this, &ThisClass::LocalControlCheck);
	
}

void UInteractionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (II_InteractiveObject* InteractiveObject = Cast<II_InteractiveObject>(OtherActor))
	{
		UpdateInteractiveObjectCount(true);
	}
	
}

void UInteractionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	if (II_InteractiveObject* InteractiveObject = Cast<II_InteractiveObject>(OtherActor))
	{
		UpdateInteractiveObjectCount(false);
	}
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UInteractionComponent::SetInteractTarget(AActor* NewInteractTarget)
{
	// update interact target
	InteractTarget = NewInteractTarget;
	// broadcast the new target
	InteractTargetUpdated.Broadcast(InteractTarget);
	
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HandleInteractiveObjectCheck();
}

void UInteractionComponent::InteractButtonPressed()
{
	if (InteractTarget == nullptr) return;
	if (Character == nullptr) return;
	
	const double CurrentTime = Character->GetWorld()->GetTimeSeconds();

	if (CurrentTime - LastInteractionTime < InteractionCooldownSeconds) return;
	LastInteractionTime = CurrentTime;
	II_InteractiveObject* InteractiveObject = Cast<II_InteractiveObject>(InteractTarget);
		
	if (InteractiveObject && Character->HasAuthority())
	{
		InteractiveObject->Interact(Character);
		// broadcast because the text might change after interact
		InteractTargetUpdated.Broadcast(InteractTarget); 
	}
	else
	{
		ServerInteractButtonPressed(InteractTarget);
		InteractTargetUpdated.Broadcast(InteractTarget);
	}
}

void UInteractionComponent::ServerInteractButtonPressed_Implementation(AActor* CurrentInteractTarget)
{
	if (II_InteractiveObject* InteractiveObject = Cast<II_InteractiveObject>(CurrentInteractTarget))
	{
		InteractiveObject->Interact(Character);
	}
}

bool UInteractionComponent::ServerInteractButtonPressed_Validate(AActor* CurrentInteractTarget)
{

	if (II_InteractiveObject* InteractiveObject = Cast<II_InteractiveObject>(CurrentInteractTarget))
	{
		const FVector CharacterLocation = Character->GetActorLocation();
		const FVector TargetLocation = CurrentInteractTarget->GetActorLocation();
	
		const double distance = FVector::Distance(CharacterLocation, TargetLocation);
	
		return distance <= InteractiveObject->GetInteractionRange();
	}
	// I dont want the player to get kicked if they somehow called this rpc without a valid interact target, so	
	return true;
	
}

void UInteractionComponent::LocalControlCheck(APlayerController* NewController)
{
	if (NewController && NewController->IsLocalController())
	{
		Character->GetCapsuleComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &UInteractionComponent::OnBeginOverlap);
		Character->GetCapsuleComponent()->OnComponentEndOverlap.AddUniqueDynamic(this, &UInteractionComponent::OnEndOverlap);

		Character->OnInteractButtonPressed.AddDynamic(this, &ThisClass::InteractButtonPressed);
		// create interact widget
		if (InteractWidgetClass)
		{
			InteractWidget = CreateWidget<UInteractWidget>(NewController, InteractWidgetClass);
			InteractWidget->BindDelegates(this);
			InteractWidget->Setup();
		}
		
	}
	
}
