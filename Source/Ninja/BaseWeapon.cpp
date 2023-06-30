// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(Root);

	collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("collision"));
	collision->SetupAttachment(Root);
	collision->SetGenerateOverlapEvents(false);
	collision->SetActive(false);

	WeaponTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponTrail"));
	WeaponTrail->SetupAttachment(Root);
	WeaponTrail->SetAutoActivate(false);

}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::AttackOn(FName FirstSocketName, FName SecondSocketName)
{
	collision->SetActive(true);
	collision->SetGenerateOverlapEvents(true);
	WeaponTrail->BeginTrails(FirstSocketName, SecondSocketName, ETrailWidthMode_FromCentre, 1.0f);
}

void ABaseWeapon::AttackOff()
{
	collision->SetActive(false);
	collision->SetGenerateOverlapEvents(false);
	WeaponTrail->EndTrails();

}
