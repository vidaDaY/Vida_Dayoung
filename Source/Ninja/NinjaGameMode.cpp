// Fill out your copyright notice in the Description page of Project Settings.


#include "NinjaGameMode.h"
#include "NinjaCharacter.h"
#include "NinjaPlayerController.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ANinjaGameMode::ANinjaGameMode()
{

	static ConstructorHelpers::FClassFinder<ANinjaCharacter> MAIN_CHARACTER(
		TEXT("/Game/KachujinBP/bp-NinjaCharacter"));

	if (MAIN_CHARACTER.Succeeded())
	{
		NinjaCharacterClass = MAIN_CHARACTER.Class;
		DefaultPawnClass = NinjaCharacterClass;
	}


	static ConstructorHelpers::FClassFinder<ANinjaPlayerController> MAIN_PLAYERCONTROLLER(
		TEXT("/Game/KachujinBP/Base/bp-NinjaPlayerController"));

	if (MAIN_PLAYERCONTROLLER.Succeeded())
	{
		NinjaPlayerControllerClass = MAIN_PLAYERCONTROLLER.Class;
		PlayerControllerClass = NinjaPlayerControllerClass;
	}

}
