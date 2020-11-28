// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My Session Game");

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

	IOnlineSubsystem* OnlineSubSystem = IOnlineSubsystem::Get();
	if (!OnlineSubSystem)
	{
		return;
	}

	SessionInterface = OnlineSubSystem->GetSessionInterface();

	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestorySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);

	}
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession(SESSION_NAME);
		}
	}
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{
	if (!Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create a session"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Created A Session %s"), *ServerName.ToString());

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

void UPuzzlePlatformsGameInstance::OnDestorySessionComplete(FName ServerName, bool Succeeded)
{
	if (!Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to destory session: %s"), *ServerName.ToString());
		return;
	}

	CreateSession(SESSION_NAME);
}

void UPuzzlePlatformsGameInstance::CreateSession(FName ServerName)
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionInterface->CreateSession(0, ServerName, SessionSettings);
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Succeeded)
{
	if (Succeeded && SessionSearch.IsValid() && Menu)
	{
		TArray<FString> ServerNames;
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

		for (FOnlineSessionSearchResult& Result : SearchResults)
		{
			FString ServerName = Result.GetSessionIdStr();

			UE_LOG(LogTemp, Warning, TEXT("Found a Session: %s"), *ServerName);

			ServerNames.Add(ServerName);
		}

		Menu->SetServerList(ServerNames);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find Session"));
	}
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface) { return; }
	if (Result == EOnJoinSessionCompleteResult::Success)
	{

		if (Menu != nullptr)
		{
			Menu->TearDown();
		}

		UEngine* Engine = GetEngine();
		APlayerController* PlayerController = GetFirstLocalPlayerController();

		if (!PlayerController)
		{
			return;
		}

		FString Address;
		if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not get a connect string"));
			return;
		}
		
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Join Session Failed"));
	}
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{

	if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) { return; }

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		//SessionSearch->QuerySettings.
		UE_LOG(LogTemp, Warning, TEXT("Starting to find session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformsGameInstance::LeaveServer()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!PlayerController)
	{
		return;
	}

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}


void UPuzzlePlatformsGameInstance::LoadMenuWidget()
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


