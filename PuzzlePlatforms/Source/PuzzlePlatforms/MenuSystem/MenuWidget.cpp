// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* MenuInterfaceSet)
{
	MenuInterface = MenuInterfaceSet;
}

void UMenuWidget::SetUp()
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

void UMenuWidget::TearDown()
{
	bIsFocusable = false;
	RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!World) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController) { return; }

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;
}