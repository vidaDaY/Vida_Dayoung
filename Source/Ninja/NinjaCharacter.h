// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ninja_GameInstance.h"
#include "PlayerData.h"
#include "NinjaCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadCall);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageCaused);

UENUM(BlueprintType)
enum class E_PlayerMode : uint8
{
	E_ThirdMode UMETA(DisplayName = "ThirdMode"),
	E_TPSMode UMETA(DisplayName = "TPSMode"),
	E_SwordMode UMETA(DisplayName = "SwordMode")
};

UCLASS(config = Game)
class ANinjaCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* TPSBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UChildActorComponent* TPSCamera;



public:


	ANinjaCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsAiming;

	UFUNCTION(BlueprintCallable, Category = "Enum")
		void SetPlayerMode(E_PlayerMode eMode);

	UPROPERTY(BlueprintReadwrite)
		E_PlayerMode ePlayerMode;

	UPROPERTY(BlueprintReadwrite)
		FPLAYERDATA playerdata;


	UPROPERTY(BlueprintReadwrite, Category = "UI")
		class UMainUserWidget* MainWidget;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf <class UMainUserWidget> MainUserWidgetClass;




protected:


	UFUNCTION(BlueprintCallable, Category = "Dash")
		void DashStart();

	UFUNCTION(BlueprintCallable, Category = "Dash")
		void DashDone();

	UFUNCTION(BlueprintCallable, Category = "Death")
		void OnDead();

	UFUNCTION(BlueprintCallable, Category = "Hit")
		void HitByEnemy(float AmountOfDamage);

	UFUNCTION(BlueprintCallable, Category = "Hit")
		void HitByBoss(float AmountOfDamage);



protected:


	void OnResetVR();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	FOnDeadCall OnDeadCall;
	FOnDamageCaused OnDamageCaused;

};

