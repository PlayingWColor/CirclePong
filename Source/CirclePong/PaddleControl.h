// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PaddleControl.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CIRCLEPONG_API UPaddleControl : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPaddleControl();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AActor * actor;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Transient, Category = "Paddle")
	float RotY;

	UPROPERTY(EditAnywhere, Category = "Paddle")
	float MaxRotY;

	UPROPERTY(EditAnywhere, Category = "Paddle")
	float MinRotY;

};
