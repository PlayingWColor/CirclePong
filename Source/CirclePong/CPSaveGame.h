// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CPSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CIRCLEPONG_API UCPSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int highscore;

	UCPSaveGame();
};
