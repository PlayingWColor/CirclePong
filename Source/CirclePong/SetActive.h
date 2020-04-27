// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SetActive.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CIRCLEPONG_API USetActive : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USetActive();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AActor* a;
	//to be used only by public variation
	void GetAllAttachedActors(AActor* parent, TArray<AActor*> &ChildActors, TArray<AActor*> &AttachedActors);

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#if WITH_EDITOR  
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	//recursive function to get all actors in hierarchy
	void GetAllAttachedActors(AActor* parent, TArray<AActor*> &ChildActors);

	UFUNCTION()
	void UpdateActive();

	void UpdateActive(bool toActive);

	UPROPERTY(EditAnywhere, Category = "Set Active")
	bool Active = true;
};
