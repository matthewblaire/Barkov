// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BARKOV_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
private:

	/*
	 *	Amount of players needed to travel to next map
	 */
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	int32 PlayerQuota = 2;

	/*
	 *	URL of map that the server will travel to when PlayerQuota is reached or when too much time has passed waiting in lobby.
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	FString MapURL;
	
};
