// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "Components/Button.h"

bool UPauseMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	if (!CancelButton || !QuitButton) { return false; }

	CancelButton->OnClicked.AddDynamic(this, &UPauseMenu::ReturnToGame);
	QuitButton->OnClicked.AddDynamic(this, &UPauseMenu::BackToMainMenu);

	return true;
}

void UPauseMenu::ReturnToGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Cancel Button Clicked"));
}

void UPauseMenu::BackToMainMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Back to main menu Clicked"));
}

void UPauseMenu::Setup()
{
}
