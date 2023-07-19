// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayerData.h"
#include "Ninja_GameInstance.generated.h"


UCLASS()
class NINJA_API UNinja_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UNinja_GameInstance();

	UPROPERTY(BlueprintReadwrite)
		FPLAYERDATA savePlayerData;

};
