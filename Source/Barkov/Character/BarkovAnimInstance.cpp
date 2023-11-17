// Fill out your copyright notice in the Description page of Project Settings.


#include "BarkovAnimInstance.h"

#include "BarkovCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

void UBarkovAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	BarkovCharacter = Cast<ABarkovCharacter>(TryGetPawnOwner());
}

void UBarkovAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (BarkovCharacter ==  nullptr)
	{
		BarkovCharacter = Cast<ABarkovCharacter>(TryGetPawnOwner());
	}
	if (BarkovCharacter == nullptr) return;

	FVector Velocity = BarkovCharacter->GetVelocity();
	Velocity.Z  = 0.f;

	Speed = Velocity.Size();
	
	bIsInAir = BarkovCharacter->GetMovementComponent()->IsFalling();
	
	bIsAccelerating = BarkovCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0;

	bJumpApexReached = BarkovCharacter->bJumpApexReached;

	bIsJumping = BarkovCharacter->bIsJumping;

	bRightFoot = BarkovCharacter->bRightFoot;
}
