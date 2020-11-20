// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "Components/Button.h"

bool UPauseMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	if (!CancelButton || !QuitButton) { return false; }

	CancelButton->OnClicked.AddDynamic(this, &UPauseMenu::TearDown);
	QuitButton->OnClicked.AddDynamic(this, &UPauseMenu::BackToMainMenu);

	return true;
}



void UPauseMenu::BackToMainMenu()
{
	if (!MenuInterface)
	{
		return;
	}

	TearDown();
	MenuInterface->LeaveServer();
}

