// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerData.generated.h"


class NINJA_API PlayerData
{
public:
	PlayerData();
	~PlayerData();
};

USTRUCT(BluePrintType)
struct FPLAYERDATA
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int nCurrentArrow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int nMaxArrow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float fCurrentHP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float fMaxHP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float fCurrentMP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float fMaxMP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int nCoin;
};
