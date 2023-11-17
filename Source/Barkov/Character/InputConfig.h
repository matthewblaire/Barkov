// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Engine/DataAsset.h"
#include "InputConfig.generated.h"

/**
 *	Add pointers to UInputAction and UInputMappingContext objects 
 */
UCLASS()
class BARKOV_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	/**
	 *  Actions go under here
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actions")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actions")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actions")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actions")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actions")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actions") 
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actions") 
	UInputAction* InteractAction;

	/**
	 * Mapping Contexts go under here
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mapping Contexts")
	UInputMappingContext* DefaultContext;
	
};
