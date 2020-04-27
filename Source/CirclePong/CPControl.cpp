// Fill out your copyright notice in the Description page of Project Settings.


#include "CPControl.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "PaddleController.h"
#include "Ball.h"
#include "SetActive.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "CPSaveGame.h"
#include "GameFramework/SaveGame.h"

// Sets default values
ACPControl::ACPControl()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1770.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;

	//Add UI
	//Widget = CreateDefaultSubobject<UWidget>(TEXT("UI"));
	//Widget->SetupAttachment(RootComponent);
	

	//allows input to work even when paused
	SetTickableWhenPaused(true);

}
#if WITH_EDITOR  
void ACPControl::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	//get currentGameMode from Enum
	
	currentGameMode = GameMode == Gamemodes::P1Survival ? 0 :
					  GameMode == Gamemodes::P2Versus ? 1 :
					  GameMode == Gamemodes::P3Elimination ? 2 :
		              GameMode == Gamemodes::P4Elimination ? 3 : -1;
					  
	//set active court depending on current game mode
	for (int i = 0; i < Courts.Num(); i++)
	{
		if (i == currentGameMode)
		{
			if (Courts[i] != nullptr)
			{
				USetActive* SetA = Courts[i]->FindComponentByClass<USetActive>();
				if (SetA != nullptr)
				{
					SetA->UpdateActive(true);
				}
			}
		}
		else
		{
			if (Courts[i] != nullptr)
			{
				USetActive* SetA = Courts[i]->FindComponentByClass<USetActive>();
				if (SetA != nullptr)
				{
					SetA->UpdateActive(false);
				}
			}
		}
	}
	//Set Points for Elimination
	if (currentGameMode > 1)
	{
		P1Points = GoalPoints;
		P2Points = GoalPoints;
		P3Points = GoalPoints;
		if (currentGameMode == 3)
			P4Points = GoalPoints;
		else
			P4Points = 0;
	}
	else//reset points for all else
	{
		P1Points = 0;
		P2Points = 0;
		P3Points = 0;
		P4Points = 0;
	}

	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
// Called to bind functionality to input
void ACPControl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	UE_LOG(LogTemp, Warning, TEXT("Bind Axis"));

	// Respond every frame to the values of the movement axes.
	InputComponent->BindAxis("P1Control", this, &ACPControl::Move1);
	InputComponent->BindAxis("P2Control", this, &ACPControl::Move2);
	InputComponent->BindAxis("P3Control", this, &ACPControl::Move3);
	InputComponent->BindAxis("P4Control", this, &ACPControl::Move4);

	//Bind Pause Button
	InputComponent->BindAction("Pause", IE_Pressed, this, &ACPControl::Pause);
}
void ACPControl::Pause()
{
	if (!IsWin && currentGameMode >= 0)
	{
		paused = !paused;
		UE_LOG(LogTemp, Warning, TEXT("Toggle Pause"));
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),paused ? 0 : 1);
	}
}

void ACPControl::Move1(float AxisValue)
{
	int32 amount = ACPControl::Paddle1.Num();
	//UE_LOG(LogTemp, Warning, TEXT("%i"), amount);
	if (currentGameMode < Paddle1.Num() && currentGameMode >= 0 && !paused)
	{
		if (Paddle1[currentGameMode] != nullptr)
			Paddle1[currentGameMode]->AdjustRot(AxisValue*0.3f);
		if(AxisValue != 0)
			P1Input = true;
		//UE_LOG(LogTemp, Warning, TEXT("%f"), AxisValue);
	}
}

void ACPControl::Move2(float AxisValue)
{
	if (currentGameMode-1 < Paddle2.Num() && currentGameMode - 1 >= 0 && currentGameMode >= 0 && !paused)
	{
		if (Paddle2[currentGameMode-1] != nullptr)
			Paddle2[currentGameMode-1]->AdjustRot(-AxisValue * 0.3f);
		if (AxisValue != 0)
			P2Input = true;
	}
}

void ACPControl::Move3(float AxisValue)
{
	if (currentGameMode - 2 < Paddle3.Num() && currentGameMode - 2 >= 0 && currentGameMode >= 0 && !paused)
	{
		if (Paddle3[currentGameMode - 2] != nullptr)
			Paddle3[currentGameMode - 2]->AdjustRot(-AxisValue * 0.3f);
		if (AxisValue != 0)
			P3Input = true;
	}
}

void ACPControl::Move4(float AxisValue)
{
	if (currentGameMode == 3 && currentGameMode >= 0 && !paused)
	{
		if (Paddle4 != nullptr)
			Paddle4->AdjustRot(AxisValue*0.3f);
		if (AxisValue != 0)
			P4Input = true;
	}
}

// Called when the game starts or when spawned
void ACPControl::BeginPlay()
{
	Super::BeginPlay();
	//don't play game until set up from menu
	NoGame();
	//load highscore
	if (UCPSaveGame* LoadedGame = Cast<UCPSaveGame>(UGameplayStatics::LoadGameFromSlot("save", 0)))
		highscore = LoadedGame->highscore;

	UE_LOG(LogTemp, Warning, TEXT("High Score : %i"), highscore);

	//Don't hide cursor after game has started
	GetWorld()->GetTimerManager().SetTimer(InputTimeHandle, this, &ACPControl::SetPlayerCursor, 1, false, 0.1f);
}

void ACPControl::SetPlayerCursor()
{
	//show mouse curser at all times
	for (TActorIterator<APlayerController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APlayerController* player = Cast<APlayerController>(*ActorItr);
		player->bShowMouseCursor = true;
		break;
	}

	GetWorld()->GetTimerManager().ClearTimer(InputTimeHandle);
}

// Called every frame
void ACPControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsWin)
	{
		FVector BallLoc = Ball->GetActorLocation();

		//What happens when ball goes out of bounds
		if (BallLoc.Size() > 500)
		{
			if (currentGameMode == 0)//Reset Game for Survival
			{
				//save highscore
				if (P1Points > highscore)
				{
					highscore = P1Points;
					if (UCPSaveGame* saveInstance = Cast<UCPSaveGame>(UGameplayStatics::CreateSaveGameObject(UCPSaveGame::StaticClass())))
					{
						saveInstance->highscore = P1Points;
						FAsyncSaveGameToSlotDelegate SavedDelegate;
						SavedDelegate.BindUObject(this, &ACPControl::SaveGameDelegateFunction);
						UGameplayStatics::AsyncSaveGameToSlot(saveInstance, "save", 0, SavedDelegate);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Save Failed!"));
					}
				}
				P1Points = 0;
				lastIncrement = 0;
				Win("");
			}
			else if (currentGameMode == 1)//Versus - Give point to opposing player
			{
				if (BallLoc.X > 0)
					P1Points++;
				else
					P2Points++;
				//Check for winners
				if (P1Points >= GoalPoints)
				{
					Win("Player 1");
				}
				if (P2Points >= GoalPoints)
				{
					Win("Player 2");
				}
			}
			else if (currentGameMode == 2)//Elimination - Remove point from loser
			{
				if (BallLoc.Z < -190)
				{
					bool more = P3Points > 0;
					P3Points = FMath::Max(P3Points - 1, 0);
					if (more && P3Points == 0)
					{
						Paddle3[currentGameMode - 2]->SetColor(FColor(128, 128, 128));
						P3Elim = true;
					}
				}
				else
				{
					if (BallLoc.X < 0)
					{
						bool more = P1Points > 0;
						P1Points = FMath::Max(P1Points - 1, 0);
						if (more && P1Points == 0)
						{
							Paddle1[currentGameMode]->SetColor(FColor(128, 128, 128));
							P1Elim = true;
						}
					}
					else
					{
						bool more = P2Points > 0;
						P2Points = FMath::Max(P2Points - 1, 0);
						if (more && P2Points == 0)
						{
							Paddle2[currentGameMode - 1]->SetColor(FColor(128, 128, 128));
							P2Elim = true;
						}
					}
				}
			}
			else if (currentGameMode == 3)//4P Elimination
			{
				if (BallLoc.Z > 350)
				{
					bool more = P4Points > 0;
					P4Points = FMath::Max(P4Points - 1, 0);
					if (more && P4Points == 0)
					{
						Paddle4->SetColor(FColor(128, 128, 128));
						P4Elim = true;
					}
				}
				else if (BallLoc.Z < -350)
				{
					bool more = P3Points > 0;
					P3Points = FMath::Max(P3Points - 1, 0);
					if (more && P3Points == 0)
					{
						Paddle3[currentGameMode - 2]->SetColor(FColor(128, 128, 128));
						P3Elim = true;
					}
				}
				else
				{
					if (BallLoc.X < 0)
					{
						bool more = P1Points > 0;
						P1Points = FMath::Max(P1Points - 1, 0);
						if (more && P1Points == 0)
						{
							Paddle1[currentGameMode]->SetColor(FColor(128, 128, 128));
							P1Elim = true;
						}
					}
					else
					{
						bool more = P2Points > 0;
						P2Points = FMath::Max(P2Points - 1, 0);
						if (more && P2Points == 0)
						{
							Paddle2[currentGameMode - 1]->SetColor(FColor(128, 128, 128));
							P2Elim = true;
						}
					}
				}
			}

			//check for winners Elimination
			if (currentGameMode > 1)
			{
				if (P1Points > 0)
				{
					if (P2Points == 0 && P3Points == 0 && P4Points == 0)
						Win("Player 1");
				}
				if (P2Points > 0)
				{
					if (P1Points == 0 && P3Points == 0 && P4Points == 0)
						Win("Player 2");
				}
				if (P3Points > 0)
				{
					if (P2Points == 0 && P1Points == 0 && P4Points == 0)
						Win("Player 3");
				}
				if (P4Points > 0)
				{
					if (P2Points == 0 && P3Points == 0 && P1Points == 0)
						Win("Player 4");
				}
			}

			if (!IsWin)
				Ball->SetBall();
		}
		//Survival Mode Ball Hits
		if (currentGameMode == 0)
		{
			if (Ball->increment > lastIncrement)
			{
				P1Points++;
				lastIncrement = Ball->increment;
			}
		}
	}
}

//Called to declare a winner
void ACPControl::Win(FString name)
{
	UE_LOG(LogTemp, Warning, TEXT("%s is the Winner!"), *name);
	IsWin = true;
	Winner = name;
}

//Called From UI to Start Game
void ACPControl::Start(bool ignoreVars, int32 toGameMode, int32 ballSpeed, int32 ballAccel, int32 startPoints)
{
	//set points and game mode
	if (!ignoreVars)
	{
		currentGameMode = toGameMode;
		GoalPoints = startPoints;

		//set ball variables
		Ball->speedIncrease = 10 + 10 * ballAccel;
		Ball->baseSpeed = 180 + Ball->speedIncrease * ballSpeed;

		P1Input = false;
		P2Input = false;
		P3Input = false;
		P4Input = false;
	}
		IsWin = false;

		P3Elim = false;
		P3Elim = false;
		P3Elim = false;
		P3Elim = false;

		Ball->SetBall();

	//set enum just in case 
	GameMode = currentGameMode == 0 ? Gamemodes::P1Survival :
			   currentGameMode == 1 ? Gamemodes::P2Versus :
			   currentGameMode == 2 ? Gamemodes::P3Elimination :
			   currentGameMode == 3 ? Gamemodes::P4Elimination : Gamemodes::P1Survival;

	//set active court depending on current game mode
	for (int i = 0; i < Courts.Num(); i++)
	{
		if (i == currentGameMode)
		{
			if (Courts[i] != nullptr)
			{
				USetActive* SetA = Courts[i]->FindComponentByClass<USetActive>();
				if (SetA != nullptr)
				{
					SetA->UpdateActive(true);
				}
			}
		}
		else
		{
			if (Courts[i] != nullptr)
			{
				USetActive* SetA = Courts[i]->FindComponentByClass<USetActive>();
				if (SetA != nullptr)
				{
					SetA->UpdateActive(false);
				}
			}
		}
	}
	//show baseCourt
	if (BaseCourt != nullptr)
	{
		USetActive* SetA = BaseCourt->FindComponentByClass<USetActive>();
		if (SetA != nullptr)
		{
			SetA->UpdateActive(true);
		}
	}

	//Set Points for Elimination
	if (currentGameMode > 1)
	{
		P1Points = GoalPoints;
		P2Points = GoalPoints;
		P3Points = GoalPoints;
		if (currentGameMode == 3)
			P4Points = GoalPoints;
		else
			P4Points = 0;
	}
	else//reset points for all else
	{
		P1Points = 0;
		P2Points = 0;
		P3Points = 0;
		P4Points = 0;
	}
}

void ACPControl::NoGame()
{
	currentGameMode = -1;
	IsWin = true;
	//hide all courts when in main menu
	for (int i = 0; i < Courts.Num(); i++)
	{
		if (Courts[i] != nullptr)
		{
			USetActive* SetA = Courts[i]->FindComponentByClass<USetActive>();
			if (SetA != nullptr)
			{
				SetA->UpdateActive(false);
			}
		}
	}
	//hide baseCourt
	if (BaseCourt != nullptr)
	{
		USetActive* SetA = BaseCourt->FindComponentByClass<USetActive>();
		if (SetA != nullptr)
		{
			SetA->UpdateActive(false);
		}
	}
	//make sure game is not paused
	paused = false;

	//put ball far away
	Ball->SetActorLocation(FVector(1000000, 0, 1000000));
}
//function for saving asyncronously
void ACPControl::SaveGameDelegateFunction(const FString & string, int32 num, bool is)
{
	if (is)
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Successful!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Failed!"));
	}
}