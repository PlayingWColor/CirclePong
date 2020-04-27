// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddleController.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteActor.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

// Sets default values
APaddleController::APaddleController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	Root->SetMobility(EComponentMobility::Movable);
}

#if WITH_EDITOR  
void APaddleController::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	this->SetActorRotation(FRotator(RotY, 0, 0));
	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void APaddleController::AdjustRot(float input)
{
	RotY = FMath::Clamp(RotY - input*1.5f, MinRotY, MaxRotY);
	//loop RotY
	if (doLoop)
	{
		if (RotY < 0)
			RotY += 360;
		if (RotY >= 360)
			RotY -= 360;
	}
	this->SetActorRotation(FRotator(RotY, 0, 0));

}

// Called when the game starts or when spawned
void APaddleController::BeginPlay()
{
	Super::BeginPlay();
	Root->SetMobility(EComponentMobility::Movable);
}

// Called every frame
void APaddleController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APaddleController::SetColor(FColor color)
{

	if (MainSprite != nullptr)
	{
		UPaperSpriteComponent * s = MainSprite->GetRenderComponent();
		if (s != nullptr)
		{
			s->SetSpriteColor(color);
		}
	}
}

