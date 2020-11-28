// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetServerName(const FText& ServerName);

	void SetServerIndex(uint32 ServerIndex, uint32 TotalNumberOfServers);

	void SetUp(class UMainMenu* Parent, uint32 Index);

protected:
	virtual bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerNameText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerIndexText;

	UPROPERTY(meta = (BindWidget))
		class UButton* ServerRowButton;

	uint32 RowIndex;

	UPROPERTY()
		class UMainMenu* MainMenu;

	UFUNCTION()
		void SetSelectedIndex();
};
