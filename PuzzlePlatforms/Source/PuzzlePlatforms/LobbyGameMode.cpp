// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("New Player joined %s"), *NewPlayer->GetFName().ToString());

	++NumberOfPlayers;
	if (NumberOfPlayers == 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Number Of Players reached 3"));

		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 10.f);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
	UE_LOG(LogTemp, Warning, TEXT("Player left number of players %d"), NumberOfPlayers);
}

void ALobbyGameMode::StartGame()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UPuzzlePlatformsGameInstance* GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	GameInstance->StartSession();
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}
