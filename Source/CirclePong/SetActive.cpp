// Fill out your copyright notice in the Description page of Project Settings.


#include "SetActive.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
USetActive::USetActive()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
	a = GetOwner();
}
#if WITH_EDITOR  
void USetActive::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	UpdateActive();

	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
void USetActive::UpdateActive(bool toActive)
{
	Active = toActive;
	
	//Get All Actors in Hierarchy
	TArray<AActor*> ChildActors;
	GetAllAttachedActors(a, ChildActors);

	//Hide in Editor
#if WITH_EDITOR  
	a->SetIsTemporarilyHiddenInEditor(!Active);

	for (AActor *c : ChildActors)
	{
		c->SetIsTemporarilyHiddenInEditor(!Active);
	}
#endif
	//Hide in Game
	UE_LOG(LogTemp, Warning, TEXT("Set Active"));
	a->SetActorHiddenInGame(!Active);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *a->GetName());

	for (AActor *c : ChildActors)
	{
		c->SetActorHiddenInGame(!Active);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *c->GetName());
	}

	// Disables collision components
	a->SetActorEnableCollision(Active);

	for (AActor *c : ChildActors)
	{
		c->SetActorEnableCollision(Active);
	}

	// Stops the Actor from ticking
	a->SetActorTickEnabled(Active);

	for (AActor *c : ChildActors)
	{
		c->SetActorTickEnabled(Active);
	}
}

void USetActive::UpdateActive()
{
	//Get All Actors in Hierarchy
	TArray<AActor*> ChildActors;
	GetAllAttachedActors(a, ChildActors);
#if WITH_EDITOR  
	//Hide in Editor
	a->SetIsTemporarilyHiddenInEditor(!Active);

	for (AActor *c : ChildActors)
	{
		c->SetIsTemporarilyHiddenInEditor(!Active);
	}
#endif
	//Hide in Game
	UE_LOG(LogTemp, Warning, TEXT("Set Active"));
	a->SetActorHiddenInGame(!Active);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *a->GetName());

	for (AActor *c : ChildActors)
	{
		c->SetActorHiddenInGame(!Active);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *c->GetName());
	}

	// Disables collision components
	a->SetActorEnableCollision(Active);

	for (AActor *c : ChildActors)
	{
		c->SetActorEnableCollision(Active);
	}

	// Stops the Actor from ticking
	a->SetActorTickEnabled(Active);

	for (AActor *c : ChildActors)
	{
		c->SetActorTickEnabled(Active);
	}
}

void USetActive::GetAllAttachedActors(AActor* parent, TArray<AActor*> &ChildActors)
{
	TArray<AActor*> AttachedActors;
	parent->GetAttachedActors(AttachedActors);

	for (AActor* c : AttachedActors)
	{
		ChildActors.Add(c);

		TArray<AActor*> AtAtActors;
		c->GetAttachedActors(AtAtActors);
		if(AtAtActors.Num() > 0)
			GetAllAttachedActors(c, ChildActors, AtAtActors);
	}
}
//To be used only when called from within above function
void USetActive::GetAllAttachedActors(AActor* parent, TArray<AActor*> &ChildActors, TArray<AActor*> &AttachedActors)
{
	for (AActor* c : AttachedActors)
	{
		ChildActors.Add(c);

		TArray<AActor*> AtAtActors;
		c->GetAttachedActors(AtAtActors);
		if (AtAtActors.Num() > 0)
			GetAllAttachedActors(c, ChildActors);
	}
}

// Called when the game starts
void USetActive::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



// Called every frame
/*
void USetActive::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}*/

