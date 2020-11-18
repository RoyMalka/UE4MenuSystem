// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MenuInterface.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	if (!HostButton || !JoinButton || !BackButton) { return false; }

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	return true;
}


void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Warning, TEXT("HostServer Button Clicked"));
	if (MenuInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Null"));
		MenuInterface->Host();
	}

}

void UMainMenu::JoinServer()
{
	if (!MenuInterface || !IPAddressField) { return; }

	FString IP = IPAddressField->GetText().ToString();
	MenuInterface->Join(IP);

}

void UMainMenu::OpenJoinMenu()
{
	if (!MenuSwitcher || !JoinMenu) { return; }

	MenuSwitcher->SetActiveWidget(JoinMenu);

}

void UMainMenu::OpenMainMenu()
{
	if (!MenuSwitcher || !MainMenu) { return; }

	MenuSwitcher->SetActiveWidget(MainMenu);

}


