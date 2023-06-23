// Fill out your copyright notice in the Description page of Project Settings.


#include "NinjaPlayerController.h"
//#include "MainWidget.h"

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

	//if (!MainUserWidget)
	//{
	//	MainUserWidget = CreateWidget<UMainWidget>(this, MainUserWidgetClass);
	//	MainUserWidget->AddToViewport();
	//}

}

