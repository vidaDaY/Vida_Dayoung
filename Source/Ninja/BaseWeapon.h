// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class NINJA_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		class UCapsuleComponent* collision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		class UParticleSystemComponent* WeaponTrail;

	UFUNCTION(BlueprintCallable, Category = "Attack")
		void AttackOn(FName FirstSocketName, FName SecondSocketName);

	UFUNCTION(BlueprintCallable, Category = "Attack")
		void AttackOff();

};
