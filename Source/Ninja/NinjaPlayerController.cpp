// Fill out your copyright notice in the Description page of Project Settings.


#include "NinjaPlayerController.h"
#include "Kismet/GameplayStatics.h"


ANinjaPlayerController::ANinjaPlayerController()
{

}

void ANinjaPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}


void ANinjaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}

