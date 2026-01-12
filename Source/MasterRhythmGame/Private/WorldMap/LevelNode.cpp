// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap/LevelNode.h"
#include "GameInstance/MyGameInstance.h"

// Sets default values
ALevelNode::ALevelNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void ALevelNode::BeginPlay()
{
	Super::BeginPlay();

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());

	switch (Level)
	{
		case ELevels::LevelOne:
		{
			if (GameInstance != nullptr)
			{
				SplineRight.bIsUnlocked = GameInstance->GetLevelOneUnlocked();
				SplineLeft.bIsUnlocked = GameInstance->GetLevelOneUnlocked();
				bIsUnlocked = GameInstance->GetLevelOneUnlocked();
				break;
			}
		}
		case ELevels::LevelTwo:
		{
				if (GameInstance != nullptr)
				{
					SplineForward.bIsUnlocked = GameInstance->GetLevelTwoUnlocked();
					SplineRight.bIsUnlocked = (GameInstance->GetLevelThreeUnlocked() && GameInstance->GetLevelFiveUnlocked());
					bIsUnlocked = GameInstance->GetLevelTwoUnlocked();
					break;
				}
		}
		case ELevels::LevelThree:
		{
				if (GameInstance != nullptr)
				{
					SplineForward.bIsUnlocked = GameInstance->GetLevelThreeUnlocked();
					SplineLeft.bIsUnlocked = (GameInstance->GetLevelTwoUnlocked() && GameInstance->GetLevelFourUnlocked());
					bIsUnlocked = GameInstance->GetLevelThreeUnlocked();
					break;
				}
		}
		case ELevels::LevelFour:
		{
			if (GameInstance != nullptr)
			{
				SplineForward.bIsUnlocked = GameInstance->GetLevelFourUnlocked();
				SplineRight.bIsUnlocked = (GameInstance->GetLevelTwoUnlocked() && GameInstance->GetLevelFourUnlocked());
				bIsUnlocked = GameInstance->GetLevelFourUnlocked();
				break;
			}
		}
		case ELevels::LevelFive:
		{
			if (GameInstance != nullptr)
			{
				SplineForward.bIsUnlocked = GameInstance->GetLevelFiveUnlocked();
				SplineLeft.bIsUnlocked = (GameInstance->GetLevelThreeUnlocked() && GameInstance->GetLevelFiveUnlocked());
				bIsUnlocked = GameInstance->GetLevelFiveUnlocked();
				break;
			}
		}
		case ELevels::LevelSix:
		{
			if (GameInstance != nullptr)
			{
				SplineLeft.bIsUnlocked = GameInstance->GetLevelFourUnlocked();
				SplineRight.bIsUnlocked = GameInstance->GetLevelFiveUnlocked();
				bIsUnlocked = GameInstance->GetLevelSixUnlocked();
			}
			break;
		}
	}
}

// Called every frame
void ALevelNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}