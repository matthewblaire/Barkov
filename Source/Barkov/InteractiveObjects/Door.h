// (c) Matthew Blaire 2023

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjectBase.h"
#include "Barkov/Interfaces/I_InteractiveObject.h"
#include "Door.generated.h"

/**
 * Multiplayer-ready doors
 */
UCLASS()
class BARKOV_API ADoor : public AActor, public II_InteractiveObject
{
	GENERATED_BODY()
public:
	ADoor();
	void StartAnimation();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	bool IsOpen() const { return bIsOpen; }
private:
	UPROPERTY(ReplicatedUsing=OnRep_bIsOpen)
	bool bIsOpen;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	double DoorSpeed = 100;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	USceneComponent* DoorHinge;

	UFUNCTION()
	void OnRep_bIsOpen();
	
	UPROPERTY(ReplicatedUsing=OnRep_YawTarget)
	double YawTarget;
	UFUNCTION()
	void OnRep_YawTarget();
	
	int32 YawTargetMultiplier = 0;
	UPROPERTY()
	FVector OpenerLocation = FVector::ZeroVector;

	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



	
	////////////////////////////////////
	/// Interactive Object Interface ///
	////////////////////////////////////
	
public:
	
	void SetGlow(bool bShouldGlow) override;
	virtual void Interact(AActor* Interactor) override;
	
	
	float GetInteractionRange() const override { return InteractionRange; }
	FText GetPromptText() const override { return PromptText; }
	UStaticMeshComponent* GetObjectVisual() const override { return ObjectVisual; }
private:

	/*	Sets up area sphere and object visual, also enables glow	*/
	void SetupInteractiveObject_Constructor();

	/* sets up query for area sphere */
	void SetupInteractiveObject_BeginPlay();
	
	/* the visual representation of the object */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UStaticMeshComponent* ObjectVisual;

	/* Text displayed when user looks at object while within interaction range */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	FText PromptText = FText::FromString(TEXT("Open"));
	
	/* Distance at which a character can interact with this object. (directly affects trigger radius) */ 
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	float InteractionRange = 100.f;

	/* Sphere that detects if a player is within interaction range */
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	USphereComponent* AreaSphere;
	
#if WITH_EDITOR
	/*
	 *	Ensures that the SphereRadius of AreaSphere gets updated when InteractionRange changes
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	////////////////////////////////////////
	/// End Interactive Object Interface ///
	////////////////////////////////////////

};
