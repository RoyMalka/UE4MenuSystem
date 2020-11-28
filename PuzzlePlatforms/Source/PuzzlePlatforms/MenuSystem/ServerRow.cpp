// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "Components/TextBlock.h"

void UServerRow::SetServerName(const FText& ServerName)
{
	ServerNameText->SetText(ServerName);
}
