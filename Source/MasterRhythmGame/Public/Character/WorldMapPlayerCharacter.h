// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WorldMapPlayerCharacter.generated.h"


class AWorldMapPath;
class AWorldMapNode;

UCLASS()
class AWorldMapPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWorldMapPlayerCharacter();

	void TryMoveInDirection(const FVector2D& InputDir);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere)
	AWorldMapNode* CurrentNode { nullptr };

	UPROPERTY(EditAnywhere)
	AWorldMapNode* TargetNode { nullptr };

	UPROPERTY(EditAnywhere)
	AWorldMapPath* CurrentPath { nullptr };

	UPROPERTY(EditAnywhere)
	float Progress { 0.0f };

	UPROPERTY(EditAnywhere)
	float MoveSpeed { 100.0f };

};
