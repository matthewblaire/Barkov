// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BarkovAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BARKOV_API UBarkovAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Character", meta=(AllowPrivateAccess))
	class ABarkovCharacter* BarkovCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta=(AllowPrivateAccess))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta=(AllowPrivateAccess))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta=(AllowPrivateAccess))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta=(AllowPrivateAccess))
	bool bJumpApexReached;

	UPROPERTY(BlueprintReadOnly, Category = "Character", meta=(AllowPrivateAccess))
	bool bRightFoot;

	UPROPERTY(BlueprintReadOnly, Category = "Character", meta=(AllowPrivateAccess))
	bool bIsJumping;
};
