// (c) Matthew Blaire 2023

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjectBase.h"
#include "Door.generated.h"

/**
 * Multiplayer-ready doors
 */
UCLASS()
class BARKOV_API ADoor : public AInteractiveObjectBase
{
	GENERATED_BODY()
public:
	ADoor();
	void StartAnimation();
	virtual void BeginPlay() override;
	virtual void Interact(AActor* Interactor) override;
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
};
