// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PaddleController.h"
#include "Ball.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CPControl.generated.h"
UENUM(BlueprintType)
namespace Gamemodes
{
	enum Type
	{
		P1Survival UMETA(DisplayName = "Survival 1P"),
		P2Versus UMETA(DisplayName = "Versus 2P"),
		P3Elimination UMETA(DisplayName = "Elimination 3P"),
		P4Elimination UMETA(DisplayName = "Elimination 4P"),

		Max				UMETA(Hidden)
	};
}
UCLASS(config = Game)
class CIRCLEPONG_API ACPControl : public APawn
{
	GENERATED_BODY()
	
	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera in front of the court */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UWidget* Widget;


public:	
	// Sets default values for this actor's properties
	ACPControl();

	FTimerHandle InputTimeHandle;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
#if WITH_EDITOR  
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int32 lastIncrement;//for counting points in survival mode

	void SetPlayerCursor();

	void SaveGameDelegateFunction(const FString & string, int32 num, bool is);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UPROPERTY(EditAnywhere, Category = "Control|GameMode")
	TEnumAsByte< Gamemodes::Type > GameMode;
	//TEnumAsByte<GameModes> GameMode = GameModes::P1Survival;//0 = single player 1 = 2 player etc.

	UPROPERTY(BlueprintReadOnly, Category = "Control|GameMode")
	int32 currentGameMode;

	//Courts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control|Courts")
	TArray<AActor*> Courts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control|Courts")
	AActor* BaseCourt;

	//Ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control|Ball")
	ABall* Ball;

	//Paddles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control|Paddle")
	TArray<APaddleController*> Paddle1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control|Paddle")
	TArray<APaddleController*> Paddle2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control|Paddle")
	TArray<APaddleController*> Paddle3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control|Paddle")
	APaddleController* Paddle4;

	//Points
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
	int32 GoalPoints = 10;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Points")
	int32 P1Points = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Points")
	int32 P2Points = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Points")
	int32 P3Points = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Points")
	int32 P4Points = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Points")
	bool IsWin = true;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Points")
	FString Winner;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "UI Control")
	bool paused;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "UI Control")
	bool P1Input;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "UI Control")
	bool P2Input;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "UI Control")
	bool P3Input;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "UI Control")
	bool P4Input;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "UI Control")
	bool P1Elim;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "UI Control")
	bool P2Elim;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "UI Control")
	bool P3Elim;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "UI Control")
	bool P4Elim;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "UI Control")
	int highscore;

	void Win(FString Winner);

	//Input Functions
	void Move1(float AxisValue);
	void Move2(float AxisValue);
	void Move3(float AxisValue);
	void Move4(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "UI Control")
	void Pause();


	//Start Game Function
	UFUNCTION(BlueprintCallable, Category = "UI Control")
	void Start(bool ignoreVars, int32 toGameMode, int32 ballSpeed, int32 ballAccel, int32 startPoints);

	UFUNCTION(BlueprintCallable, Category = "UI Control")
	void NoGame();

};