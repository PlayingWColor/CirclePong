// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PaperSpriteActor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaddleController.generated.h"

UCLASS()
class CIRCLEPONG_API APaddleController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaddleController();

	UPROPERTY()
	USceneComponent* Root;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
#if WITH_EDITOR  
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Paddle")
	APaperSpriteActor* MainSprite;

	UFUNCTION()
	void AdjustRot(float input);

	UPROPERTY(EditAnywhere, Category = "Paddle")
	bool doLoop = false;

	UPROPERTY(EditAnywhere, Category = "Paddle")
	float RotY;

	UPROPERTY(EditAnywhere, Category = "Paddle")
	float MinRotY;

	UPROPERTY(EditAnywhere, Category = "Paddle")
	float MaxRotY;



	UFUNCTION()
	void SetColor(FColor color);

};
