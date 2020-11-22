// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(Exec)
		virtual void Host() override;

	UFUNCTION(Exec)
		void Join(const FString& Address) override;

	UFUNCTION(Exec)
		void LeaveServer() override;


	UFUNCTION(BlueprintCallable)
		void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
		void LoadPauseMenu();

private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> PauseMenuClass;

	class UMainMenu* Menu;
	IOnlineSessionPtr SessionInterface;

	void OnCreateSessionComplete(FName ServerName, bool Succeeded);
	void OnDestorySessionComplete(FName ServerName, bool Succeeded);
	void CreateSession(FName ServerName);
};
