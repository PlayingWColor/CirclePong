// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddleControl.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPaddleControl::UPaddleControl()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPaddleControl::BeginPlay()
{
	Super::BeginPlay();

	actor = GetOwner();
	RotY = actor->GetActorRotation().Pitch;
	// ...
}


// Called every frame
void UPaddleControl::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
