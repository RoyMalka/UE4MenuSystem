// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Consturctor"));
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	static ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuBPClass(TEXT("/Game/MenuSystem/WBP_PauseMenu"));

	if (!MenuBPClass.Class || !PauseMenuBPClass.Class)
	{
		return;
	}

	MenuClass = MenuBPClass.Class;
	PauseMenuClass = PauseMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init"));
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (Menu != nullptr)
	{
		Menu->TearDown();
	}

	UEngine* Engine = GetEngine();
	if (Engine)
	{
		Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, TEXT("Host"));
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	if (Menu != nullptr)
	{
		Menu->TearDown();
	}

	UEngine* Engine = GetEngine();
	if (Engine)
	{
		Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, FString::Printf(TEXT("Joining: %s"), *Address));
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!PlayerController)
	{
		return;
	}

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!MenuClass) { return; }

	Menu = CreateWidget<UMainMenu>(this, MenuClass);

	if (!Menu) { return; }
	Menu->SetMenuInterface(this);
	Menu->SetUp();
}

void UPuzzlePlatformsGameInstance::LoadPauseMenu()
{
	if (!PauseMenuClass) { return; }

	UMenuWidget* PauseMenu = CreateWidget<UMenuWidget>(this, PauseMenuClass);

	if (!PauseMenu) { return; }
	PauseMenu->SetMenuInterface(this);
	PauseMenu->SetUp();
}
