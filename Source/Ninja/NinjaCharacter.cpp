// Copyright Epic Games, Inc. All Rights Reserved.

#include "NinjaCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Ninja_GameInstance.h"
#include "MainUserWidget.h"
#include "NinjaPlayerController.h"


ANinjaCharacter::ANinjaCharacter()
{

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;

	TPSBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPSBoom"));
	TPSBoom->SetupAttachment(RootComponent);
	TPSBoom->TargetArmLength = 300.0f;
	TPSBoom->bUsePawnControlRotation = true;
	TPSBoom->bDoCollisionTest = true;

	// Create a follow camera
	TPSCamera = CreateDefaultSubobject<UChildActorComponent>(TEXT("TPSCamera"));
	TPSCamera->SetupAttachment(TPSBoom, USpringArmComponent::SocketName);

	static ConstructorHelpers::FClassFinder<UMainUserWidget> WidgetClass(TEXT("/Game/KachujinBP/Widget/bp-Main"));
	if (WidgetClass.Succeeded())
	{
		MainUserWidgetClass = WidgetClass.Class;
	}

	OnDeadCall.AddDynamic(this, &ANinjaCharacter::OnDead);

}


void ANinjaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ANinjaCharacter::DashStart);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &ANinjaCharacter::DashDone);

	PlayerInputComponent->BindAxis("MoveForward", this, &ANinjaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANinjaCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ANinjaCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ANinjaCharacter::LookUpAtRate);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ANinjaCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ANinjaCharacter::TouchStopped);
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ANinjaCharacter::OnResetVR);
}

void ANinjaCharacter::BeginPlay()
{
	Super::BeginPlay();

	UNinja_GameInstance* pNinjaGameInstance;

	pNinjaGameInstance = Cast<UNinja_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	playerdata = pNinjaGameInstance->savePlayerData;


	if (MainUserWidgetClass)
	{
		MainWidget = CreateWidget<UMainUserWidget>(GetWorld(), MainUserWidgetClass);
		if (MainWidget)
		{
			MainWidget->BindHpBar(playerdata.fCurrentHP);
			MainWidget->AddToViewport();

		}
	}

}

void ANinjaCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UNinja_GameInstance* pNinjaGameInstance;

	pNinjaGameInstance = Cast<UNinja_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	pNinjaGameInstance->savePlayerData = playerdata;
}


void ANinjaCharacter::OnResetVR()
{

	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ANinjaCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ANinjaCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}


void ANinjaCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANinjaCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ANinjaCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ANinjaCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void ANinjaCharacter::SetPlayerMode(E_PlayerMode eMode)
{
	APlayerController* OurPlayer = UGameplayStatics::GetPlayerController(this, 0);
	AActor* TPS{};
	TPS = TPSCamera->GetChildActor();


	if (eMode == E_PlayerMode::E_ThirdMode)
	{
		OurPlayer->SetViewTargetWithBlend(this, 0.2f);
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = 850.0f;
		bIsAiming = false;
	}
	else if (eMode == E_PlayerMode::E_TPSMode)
	{
		OurPlayer->SetViewTargetWithBlend(TPS, 0.2f);
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->MaxWalkSpeed = 450.0f;
		bIsAiming = true;
	}

}

void ANinjaCharacter::HitByEnemy(float AmountOfDamage)
{
	if (playerdata.fCurrentHP > 0)
	{
		playerdata.fCurrentHP -= AmountOfDamage;

		MainWidget->BindHpBar(playerdata.fCurrentHP);

		if (playerdata.fCurrentHP <= 0)
		{
			OnDeadCall.Broadcast();
		}
	}
}

void ANinjaCharacter::DashStart()
{
	if (!(GetCharacterMovement()->IsFalling()) && !bIsAiming)
	{
		GetCharacterMovement()->MaxWalkSpeed = 1500.0f;

		CameraBoom->bEnableCameraLag = true;

		CameraBoom->CameraLagSpeed = 3.0;

		FTimerHandle WaitHandle;
		float WaitTime;

		WaitTime = 0.8f;

		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{

				GetCharacterMovement()->MaxWalkSpeed = 600.0f;
				CameraBoom->CameraLagSpeed = 15.0;
				GetWorld()->GetTimerManager().ClearTimer(WaitHandle);

			}), WaitTime, false);
	}
}

void ANinjaCharacter::DashDone()
{

	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	CameraBoom->CameraLagSpeed = 15.0;
}

void ANinjaCharacter::OnDead()
{

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->JumpZVelocity = 0.f;
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	TPSBoom->bUsePawnControlRotation = false;
	CameraBoom->bUsePawnControlRotation = false;

}

