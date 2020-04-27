// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Components/SphereComponent.h"
#include "PaperSprite.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);
	Collider->SetSimulatePhysics(true);
	Collider->SetConstraintMode(EDOFMode::Default);
	Collider->SetNotifyRigidBodyCollision(true);

	Collider->OnComponentHit.AddDynamic(this, &ABall::OnHit);

	Sprite = CreateDefaultSubobject<UPaperSprite>(TEXT("Sprite"));


	speed = baseSpeed;
	//Sprite->SetupAttachment(Collider, USphereComponent::SocketName);
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
}

void ABall::SetBall()
{
	//set or reset variables
	SetActorLocation(FVector::ZeroVector);
	increment = 0;
	beforeStart = 5;
	
	speed = baseSpeed;

	Collider->SetPhysicsLinearVelocity(FVector::ZeroVector);

	FVector randDir = FVector(FMath::RandRange(-1, 1), 0, FMath::RandRange(-1, 1));
	randDir.Normalize();

	Collider->AddForce(randDir * speed * 4000);
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (beforeStart > 0)
	{
		//count down
		beforeStart -= DeltaTime;
		//revolve around center
		if (beforeStart < 4.8f)
		{
			FVector dir = FVector(FMath::RandRange(-10, 10), 0, FMath::RandRange(-10, 10)) - GetActorLocation();
			dir.Normalize();

			Collider->AddForce(dir * 1000000);
		}
		//start level
		if (beforeStart <= 0)
		{
			//Collider->AddForce(FVector(-1, 0, 0) * speed);
			FVector velDir = Collider->GetPhysicsLinearVelocity();
			velDir.Normalize();
			LastVelocity = velDir * speed;
			Collider->SetPhysicsLinearVelocity(LastVelocity);
		}
	}
	else
	{
		//make sure ball speed stays constant
		if (Collider->GetPhysicsLinearVelocity() != LastVelocity)
			Collider->SetPhysicsLinearVelocity(LastVelocity);

		//make sure hit only happens once per hit
		if (hitDelay > 0)
			hitDelay -= DeltaTime;
		if (hitDelay < 0)
			hitDelay = 0;
	}
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + LastVelocity, FColor::Red, false, -1, 0, 5);
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + Collider->GetPhysicsLinearVelocity(), FColor::Yellow, true, -1, 0, 5);
}
//Bounce ball on collision
void ABall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && hitDelay == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("HIT"));
		hitDelay = 0.1f;

		increment++;

		float formula = -(-(increment*0.5f + 1) / FMath::Pow(increment * 0.2f + 1, 2));

		UE_LOG(LogTemp, Warning, TEXT("%f"), formula);

		//adjust speed
		speed += speedIncrease* formula;

		//bounce off in opposite direction with slight random variance in angle
		float angleDis = FMath::RandRange(4, 12);
		int randDir = 1;
		if (FMath::RandBool())
			randDir = -1;
		FRotator AngleVar = FRotator(angleDis * randDir, 0, 0);

		//FVector newVel = (LastVelocity/LastVelocity.Size()) * -speed;

		FVector newVel = (NormalImpulse/ NormalImpulse.Size()) * speed;

		newVel = AngleVar.RotateVector(newVel);
		Collider->SetPhysicsLinearVelocity(newVel);

		LastVelocity = newVel;
	}
}

