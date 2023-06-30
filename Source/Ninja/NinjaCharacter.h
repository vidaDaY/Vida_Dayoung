// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "BaseWeapon.h"
#include "S_PlayerData.h"
#include "NinjaCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadCall);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsAiming;

	UFUNCTION(BlueprintCallable, Category = "Enum")
		void SetPlayerMode(E_PlayerMode eMode);

	UFUNCTION(BlueprintCallable, Category = "Hit")
		void HitByEnemy();

	UFUNCTION(BlueprintCallable, Category = "Hit")
		void HitByBoss();

	UPROPERTY(BlueprintReadwrite)
		E_PlayerMode ePlayerMode;

	UPROPERTY(BlueprintReadwrite)
		FPLAYERDATA PlayerData;

	UPROPERTY(BlueprintReadwrite)
		AActor* DamageEffect;

	UPROPERTY(BlueprintReadwrite)
		TSubclassOf<AActor> DamageEffectClass;

protected:

	void OnResetVR();

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION(BlueprintCallable, Category = "Dash")
		void DashStart();

	UFUNCTION(BlueprintCallable, Category = "Dash")
		void DashDone();

	UFUNCTION(BlueprintCallable, Category = "Death")
		void OnDead();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FOnDeadCall OnDeadCall;
};

