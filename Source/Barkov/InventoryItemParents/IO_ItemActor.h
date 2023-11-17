// (c) Matthew Blaire 2023

#pragma once

#include "CoreMinimal.h"
#include "Barkov/Interfaces/I_InteractiveObject.h"
#include "Core/Actors/Parents/A_ItemActor.h"
#include "IO_ItemActor.generated.h"

/**
 * 
 */
UCLASS()
class BARKOV_API AIO_ItemActor : public AA_ItemActor, public II_InteractiveObject
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	AIO_ItemActor();
	////////////////////////////////////
	/// Interactive Object Interface ///
	////////////////////////////////////
	
public:
	
	void SetGlow(bool bShouldGlow) override;

	
	virtual void Interact(AActor* Interactor) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Interact(AActor* Interactor, UAC_Inventory* InteractorInventory);
	
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
	class USphereComponent* AreaSphere;
	
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
