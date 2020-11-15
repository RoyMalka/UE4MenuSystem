// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MenuInterface.h"
#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	if (!HostButton || !JoinButton) { return false; }

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostButtonClicked);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinButtonClicked);

	return true;
}


void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterfaceSet)
{
	MenuInterface = MenuInterfaceSet;
}

void UMainMenu::SetUp()
{
	bIsFocusable = true;
	AddToViewport();

	UWorld* World = GetWorld();
	if (!World) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController) { return; }

	FInputModeUIOnly InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetWidgetToFocus(TakeWidget());
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;
}

void UMainMenu::TearDown()
{
	UE_LOG(LogTemp, Warning, TEXT("SetInput"));
	bIsFocusable = false;
	RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!World) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController) { return; }

	UE_LOG(LogTemp, Warning, TEXT("SetInput"));
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;


}

void UMainMenu::OnHostButtonClicked()
{
	if (MenuInterface)
	{
		MenuInterface->Host();
	}

	UE_LOG(LogTemp, Warning, TEXT("Host Button Clicked"));
}

void UMainMenu::OnJoinButtonClicked()
{

	UE_LOG(LogTemp, Warning, TEXT("Join Button Clicked"));
}
