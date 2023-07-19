// Fill out your copyright notice in the Description page of Project Settings.


#include "Ninja_GameInstance.h"

UNinja_GameInstance::UNinja_GameInstance()
{
	savePlayerData.fCurrentHP = 1.0f;
	savePlayerData.fMaxHP = 1.0f;
	savePlayerData.fCurrentMP = 1.0f;
	savePlayerData.fMaxMP = 1.0f;
	savePlayerData.nCurrentArrow = 30;
	savePlayerData.nMaxArrow = 30;
	savePlayerData.nCoin = 0.0f;
}