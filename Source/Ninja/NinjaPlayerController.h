// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NinjaPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NINJA_API ANinjaPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	ANinjaPlayerController();


	virtual void OnPossess(APawn* aPawn) override;


protected:
	virtual void BeginPlay() override;



};
                        