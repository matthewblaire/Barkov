// Fill out your copyright notice in the Description page of Project Settings.


#include "BarkovCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputConfig.h"
#include "Barkov/InteractiveObjects/InteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABarkovCharacter::ABarkovCharacter()
{
	
	PrimaryActorTick.bCanEverTick = false;
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("Interaction Component");
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	///  Character body attachment slots
	HatSlot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hat Slot"));
	HairSlot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair Slot"));
	ArmorSlot = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Armor Slot"));
	BackSlot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back Slot"));
	
	HatSlot->SetupAttachment(GetMesh(), TEXT("headSocket"));
	HairSlot->SetupAttachment(GetMesh(), TEXT("headSocket"));
	ArmorSlot->SetupAttachment(GetMesh());
	BackSlot->SetupAttachment(GetMesh(), TEXT("backSocket"));

	// TODO: Player inventory
	
}

void ABarkovCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bIsSprinting);
}

// Called when the game starts or when spawned
void ABarkovCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetWorld()->GetFirstLocalPlayerFromController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSystem->AddMappingContext(InputConfig->DefaultContext, 0);
		}
	}
	GetCharacterMovement()->MaxWalkSpeed = this->MaxSpeedWalk;

	// make sure armor in slot is moving with character mesh
	ArmorSlot->SetLeaderPoseComponent(GetMesh());
}



// Called every frame
void ABarkovCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABarkovCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!InputConfig) return;
	
	Input->BindAction(InputConfig->JumpAction, ETriggerEvent::Started, this, &ThisClass::JumpPressed);
	Input->BindAction(InputConfig->JumpAction, ETriggerEvent::Completed, this, &ThisClass::JumpReleased);
	Input->BindAction(InputConfig->MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	Input->BindAction(InputConfig->LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	Input->BindAction(InputConfig->SprintAction, ETriggerEvent::Started, this, &ThisClass::SprintPressed);
	Input->BindAction(InputConfig->SprintAction, ETriggerEvent::Completed, this, &ThisClass::SprintReleased);
	Input->BindAction(InputConfig->InteractAction, ETriggerEvent::Completed, this, &ThisClass::InteractButtonPressed);
	
}

void ABarkovCharacter::JumpPressed()
{
	
	if (!GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->bNotifyApex = true;
		bJumpApexReached = false;
		bIsJumping = true;
		bRightFoot = UKismetMathLibrary::RandomBool();
		Jump();
	}
	
}

void ABarkovCharacter::JumpReleased()
{
	StopJumping();
	bIsJumping = false;
}

void ABarkovCharacter::Move(const FInputActionValue&  Value)
{
	if (Controller != nullptr)
	{
		const FVector2D MoveValue = Value.Get<FVector2D>();
		
		// forward/backward
		if (MoveValue.Y != 0.f)
		{
			const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
			const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
			AddMovementInput(Direction, MoveValue.Y);
		}

		// left/right
		if (MoveValue.X != 0.f)
		{
			const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
			const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
			AddMovementInput(Direction, MoveValue.X);
		}
	}
}

void ABarkovCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookValues = Value.Get<FVector2D>();
	AddControllerYawInput(LookValues.X);
	AddControllerPitchInput(LookValues.Y);
}

void ABarkovCharacter::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	bJumpApexReached = true;
}

void ABarkovCharacter::SprintPressed()
{
	if (HasAuthority())
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedSprint;
		bIsSprinting = true;
	}
	else
	{
		ServerSprintButtonPressed();
	}
	
}

void ABarkovCharacter::SprintReleased()
{
	if (HasAuthority())
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedWalk;
		bIsSprinting = false;	
	}
	else
	{
		ServerSprintButtonReleased();
	}
}

void ABarkovCharacter::OnRep_bIsSprinting()
{
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedSprint;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedWalk;
	}
}



void ABarkovCharacter::ServerSprintButtonReleased_Implementation()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeedWalk;
}

void ABarkovCharacter::ServerSprintButtonPressed_Implementation()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeedSprint;
}

void ABarkovCharacter::InteractButtonPressed()
{
	
	InteractionComponent->InteractButtonPressed();

	
}






