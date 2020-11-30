// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MainMenu.h"

bool UServerRow::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	ServerRowButton->OnClicked.AddDynamic(this, &UServerRow::SetSelectedIndex);

	return true;

}
void UServerRow::SetServerName(const FText& ServerName)
{
	ServerNameText->SetText(ServerName);
}

void UServerRow::SetHostUserName(const FText& HostUser)
{
	HostUserName->SetText(HostUser);
}

void UServerRow::SetSelectedIndex()
{
	if (MainMenu)
	{
		MainMenu->SelectIndex(RowIndex);
	}
}

void UServerRow::SetConnectionFraction(uint32 CurrentNumOfPlayers, uint32 TotalNumberOfPlayers)
{
	FString Text = FString::Printf(TEXT("%d/%d"), CurrentNumOfPlayers, TotalNumberOfPlayers);
	ConnectionFraction->SetText(FText::FromString(Text));
}

void UServerRow::SetUp(UMainMenu* Parent, uint32 Index)
{
	MainMenu = Parent;
	RowIndex = Index;
}
