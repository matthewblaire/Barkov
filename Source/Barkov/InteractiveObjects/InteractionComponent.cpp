// (c) Matthew Blaire 2023


#include "InteractionComponent.h"

#include "InteractiveObjectBase.h"
#include "Barkov/Character/BarkovCharacter.h"
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
		UE_LOG(LogTemp, Warning, TEXT("NO Interact"));
		InteractiveObjectsNearby = 0; // just in case it's below 0 lol
		if (InteractWidget && InteractWidget->IsVisible())
		{
			InteractWidget->HidePrompt();
		}
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

		if (AInteractiveObjectBase* InteractiveObject = Cast<AInteractiveObjectBase>(Hit.GetActor()))
		{
			
			InteractTarget = InteractiveObject;
			if (InteractWidget)
			{
				InteractWidget->DisplayPrompt(InteractiveObject->GetPromptText());
			}
		}
		else
		{
			if (InteractWidget && InteractWidget->IsVisible())
			{
				InteractWidget->HidePrompt();
			}
		}
	}
	else
	{
		
		InteractTarget = nullptr;
		if (InteractWidget && InteractWidget->IsVisible())
		{
			InteractWidget->HidePrompt();
		}
	}

	//draw debug line
	if (bDrawDebugLine)	DrawDebugLine(GetWorld(), Character->GetActorLocation(), TraceEnd, FColor::Red, false, -1.f, 0, 5.f);
}

void UInteractionComponent::OnRegister()
{
	Super::OnRegister();
	
	Character = Cast<ABarkovCharacter>(GetOwner()); // set up reference to owner
}

void UInteractionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AInteractiveObjectBase>(OtherActor))
	{
		UpdateInteractiveObjectCount(true);
	}
}

void UInteractionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AInteractiveObjectBase>(OtherActor))
	{
		UpdateInteractiveObjectCount(false);
	}
}




void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HandleInteractiveObjectCheck();
	if (bLocalControlChecked == false) LocalControlCheck();
}

void UInteractionComponent::InteractButtonPressed()
{
	if (InteractTarget != nullptr)
	{
		if (GetOwner()->HasAuthority())
		{
			InteractTarget->Interact(GetOwner());
		}
		else
		{
			ServerInteractButtonPressed(InteractTarget);
		}
	}
}

void UInteractionComponent::ServerInteractButtonPressed_Implementation(AInteractiveObjectBase* Target)
{
	Target->Interact(Character);
}

bool UInteractionComponent::ServerInteractButtonPressed_Validate(AInteractiveObjectBase* Target)
{
	if (Target == nullptr) return false;
	const FVector CharacterLocation = Character->GetActorLocation();
	const FVector TargetLocation = Target->GetActorLocation();
	const double distance = FVector::Distance(CharacterLocation, TargetLocation);
	return distance <= Target->GetInteractionRange();
}

void UInteractionComponent::LocalControlCheck()
{
	if (IsValid(Character->GetController()))
	{
		if (Character->IsLocallyControlled())
		{
			Character->GetCapsuleComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &UInteractionComponent::OnBeginOverlap);
			Character->GetCapsuleComponent()->OnComponentEndOverlap.AddUniqueDynamic(this, &UInteractionComponent::OnEndOverlap);

			// create interact widget
			InteractWidget = CreateWidget<UInteractWidget>(GetWorld(), InteractWidgetClass);
			InteractWidget->Setup();
		}
		bLocalControlChecked = true;
		SetComponentTickEnabled(false);	
	}
}
