// Copyright Epic Games, Inc. All Rights Reserved.

#include "NinjaCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NinjaPlayerController.h"


//////////////////////////////////////////////////////////////////////////
// UE_LOG(LogTemp, Log, TEXT("Okay Bye"));

ANinjaCharacter::ANinjaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	TPSBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPSBoom"));
	TPSBoom->SetupAttachment(RootComponent);
	TPSBoom->TargetArmLength = 300.0f;
	TPSBoom->bUsePawnControlRotation = true; 
	TPSBoom->bDoCollisionTest = true;

	// Create a follow camera
	TPSCamera = CreateDefaultSubobject<UChildActorComponent>(TEXT("TPSCamera"));
	TPSCamera->SetupAttachment(TPSBoom, USpringArmComponent::SocketName);


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


//////////////////////////////////////////////////////////////////////////
// Input

void ANinjaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ANinjaCharacter::DashStart);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &ANinjaCharacter::DashDone);



	PlayerInputComponent->BindAxis("MoveForward", this, &ANinjaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANinjaCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ANinjaCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ANinjaCharacter::LookUpAtRate);


	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ANinjaCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ANinjaCharacter::TouchStopped);


	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ANinjaCharacter::OnResetVR);
}


void ANinjaCharacter::OnResetVR()
{
	// If Ninja is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in Ninja.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
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
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANinjaCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ANinjaCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ANinjaCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


//void ANinjaCharacter::DashStart()
//{
//	
//}

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
	//CameraBoom->bEnableCameraLag = false;
	CameraBoom->CameraLagSpeed = 15.0;
}


