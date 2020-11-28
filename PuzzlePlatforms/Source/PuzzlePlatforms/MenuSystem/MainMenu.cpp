// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MenuInterface.h"
#include "Components/Button.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"
#include "Components/PanelWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ServerRow.h"


UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (ServerRowBPClass.Class)
	{
		ServerRowClass = ServerRowBPClass.Class;
	}
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	if (!HostButton || !JoinButton || !BackButton) { return false; }

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	return true;
}


void UMainMenu::HostServer()
{
	if (MenuInterface)
	{
		MenuInterface->Host();
	}

}

void UMainMenu::JoinServer()
{

	if (!MenuInterface) { return; }

	//FString IP = IPAddressField->GetText().ToString();
	MenuInterface->Join("");

}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	if (ServerRowClass)
	{
		ServersList->ClearChildren();
		for (FString& ServerName : ServerNames)
		{
			auto ServerRow = CreateWidget<UServerRow>(GetWorld(), ServerRowClass);
			ServerRow->SetServerName(FText::FromString(ServerName));
			ServersList->AddChild(ServerRow);
		}	
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!MenuSwitcher || !JoinMenu || !MenuInterface) { return; }

	MenuSwitcher->SetActiveWidget(JoinMenu);

	MenuInterface->RefreshServerList();

}

void UMainMenu::OpenMainMenu()
{
	if (!MenuSwitcher || !MainMenu) { return; }

	MenuSwitcher->SetActiveWidget(MainMenu);

}

void UMainMenu::QuitGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit Button Clicked"));
	UWorld* World = GetWorld();
	if (!World) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController) { return; }

	PlayerController->ConsoleCommand("quit");
}


