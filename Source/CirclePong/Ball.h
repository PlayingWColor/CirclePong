// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class CIRCLEPONG_API ABall : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* Collider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPaperSprite* Sprite;

public:	
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector Velocity;
	FVector LastVelocity;
	float hitDelay = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Ball")
	float beforeStart = 5;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Ball")
	float speed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Ball")
	float increment;//used to determine how much the speed needs to increase

	UPROPERTY(EditAnywhere, Category = "Ball")
	float baseSpeed = 100;

	UPROPERTY(EditAnywhere, Category = "Ball")
	float speedIncrease = 50;

	UPROPERTY(EditAnywhere, Category = "Ball")
	bool didHit;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void SetBall();

};
