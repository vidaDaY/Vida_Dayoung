// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "NinjaGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NINJA_API ANinjaGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ANinjaGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
 		TSubclassOf<class ANinjaCharacter> NinjaCharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class ANinjaPlayerController> NinjaPlayerControllerClass;


};
