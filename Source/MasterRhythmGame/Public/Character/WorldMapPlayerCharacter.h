// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Public/CoreTypes.h"
#include "WorldMapPlayerCharacter.generated.h"


class ALevelNode;
class ALevelPath;
class USplineComponent;
class UTimelineComponent;

UCLASS()
class AWorldMapPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWorldMapPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Timeline progress callback — only float because Timeline delegate supplies only a float
	UFUNCTION()
	void HandleTimelineProgress(float Value);

	UFUNCTION()
	void HandleTimelineFinished();

	UFUNCTION()
	void MoveForward(EDirectionWorldMap InDirection);

	UFUNCTION()
	void MoveBackward();

	// --- Level Node Ref ---
	TObjectPtr<ALevelNode> GetLevelNodeRef() const { return LevelNodeRef; }
	void SetLevelNodeRef(TObjectPtr<ALevelNode> InLevelNodeRef) { LevelNodeRef = InLevelNodeRef; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAudioComponent> AudioComponent { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComponent { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UTimelineComponent> Timeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<ALevelNode> LevelNodeClass { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<ALevelNode> LevelNodeRef { nullptr };

	// Current direction used during timeline movement
	EDirectionWorldMap CurrentDirection { EDirectionWorldMap::Forward };
};
