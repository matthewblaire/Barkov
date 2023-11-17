// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BarkovCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractButtonPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPossession, APlayerController*, NewPlayerController);
class UInteractionComponent;
class UInteractWidget;
class USpringArmComponent;
UCLASS()
class BARKOV_API ABarkovCharacter : public ACharacter 
{
	GENERATED_BODY()
	
public:
	
	ABarkovCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	class UCameraComponent* GetCam() {return Camera;}
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	bool bJumpApexReached = false;
	bool bIsJumping = false;
	bool bRightFoot = false;

	/* Used by the interaction component */
	FInteractButtonPressed OnInteractButtonPressed;
	
	/* Used by the interaction component to ensure local control is checked only after player controller exists */
	FOnPossession OnPossession;
	
private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	class UInputConfig* InputConfig;

	UFUNCTION()
	void JumpPressed();

	UFUNCTION()
	void JumpReleased();

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	virtual void NotifyJumpApex() override;

	UFUNCTION()
	void SprintPressed();

	UFUNCTION()
	void SprintReleased();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	float MaxSpeedSprint = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	float MaxSpeedWalk = 450.f;

	// body attachments
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	UStaticMeshComponent* HairSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	UStaticMeshComponent* HatSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	USkeletalMeshComponent* ArmorSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	UStaticMeshComponent* BackSlot;

	UPROPERTY(ReplicatedUsing=OnRep_bIsSprinting)
	bool bIsSprinting = false;

	UFUNCTION()
	void OnRep_bIsSprinting();
	
	UFUNCTION(Server, Reliable)
	void ServerSprintButtonPressed();

	UFUNCTION(Server, Reliable)
	void ServerSprintButtonReleased();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	UInteractionComponent* InteractionComponent;
public:
	UInteractionComponent* GetInteractionComponent() {return InteractionComponent;}
	virtual void Restart() override;

	
private:

	/*
	 *	broadcasts OnInteractButtonPressed delegate
	 */
	UFUNCTION()
	void InteractButtonPressed();



	
	
};


