// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "NinjaCharacter.h"
#include "Kismet/GameplayStatics.h"

UMainUserWidget::UMainUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsNativeConstructorSet = false;

}

void UMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();


	if (!bIsNativeConstructorSet)
	{
		bIsNativeConstructorSet = true;

		HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
		ManaBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ManaBar")));
		CoinCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("CoinCount")));

	}


	NinjaCharacter = Cast<ANinjaCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}


void UMainUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);


	if (!NinjaCharacter)
	{
		return;
	}

	else
	{

		int16 nCoinCount = NinjaCharacter->playerdata.nCoin;
		float fMPPercentage = NinjaCharacter->playerdata.fCurrentMP;

		BindMpBar(fMPPercentage);
		BindCoin(nCoinCount);

	}


}

void UMainUserWidget::BindHpBar(float fHPPercentage)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(fHPPercentage);
	}
}

void UMainUserWidget::BindMpBar(float fMPPercentage)
{
	if (ManaBar)
	{
		ManaBar->SetPercent(fMPPercentage);
	}
}

void UMainUserWidget::BindCoin(int nCoin)
{
	CoinCount->SetText(FText::AsNumber(nCoin));
}


