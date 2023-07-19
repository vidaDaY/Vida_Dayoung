// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NinjaCharacter.h"
#include "MainUserWidget.generated.h"

//class UNinjaCharacter;

UCLASS()
class NINJA_API UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainUserWidget (const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "ProgressBar")
		void BindHpBar(float fHPPercentage);

	UFUNCTION(BlueprintCallable, Category = "ProgressBar")
		void BindMpBar(float fMPPercentage);

	UFUNCTION(BlueprintCallable, Category = "Coin")
		void BindCoin(int nCoin);

private:
	UPROPERTY( meta = ( BindWidget ))
		class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ManaBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CoinCount;

	UPROPERTY(meta = (BindWidget))
		class UImage* HpImage;

	UPROPERTY(meta = (BindWidget))
		class UImage* CoinImage;

	bool bIsNativeConstructorSet;

	ANinjaCharacter* NinjaCharacter;
};
