// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual bool Initialize() override;

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);

	void SetUp();
	void TearDown();
	//virtual void OnLevelRemovedFromWorld(class ULevel* InLevel, UWorld* InWorld);

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;


	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UFUNCTION()
		void OnHostButtonClicked();

	UFUNCTION()
		void OnJoinButtonClicked();

	IMenuInterface* MenuInterface;
};
