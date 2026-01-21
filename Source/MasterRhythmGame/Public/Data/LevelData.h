// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CoreTypes.h"
#include "Engine/DataAsset.h"
#include "LevelData.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API ULevelData : public UDataAsset
{
	GENERATED_BODY()

public:

	/**
	 * Music
	 * 
	 * RootNote
	 * 
	 * Scale
	 * 
	 * SkeletalMesh
	 * 
	 * BPM
	 * 
	 * EPartFinish NumSegments
	 * 
	 * Health Bars 1-5
	 * 
	 */

	//Music
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* Music { nullptr };

	//BPM
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BPM { 120.f };

	//RootNote
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ENote RootNote { ENote::C };

	//Scale
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EScale Scale { EScale::Major };

	//Amount of Segments
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPartFinish NumSegments { EPartFinish::Three };

	//Health Points per Note in Melody
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 HealthPointsPerNote { 5 };

	//Amount of Notes in Melody1
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumNotesMelody1 { 1 };

	//Amount of Notes in Melody2
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumNotesMelody2 { 1 };

	//Amount of Notes in Melody3
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumNotesMelody3 { 1 };

	//Amount of Notes in Melody4
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumNotesMelody4 { 1 };

	//Amount of Notes in Melody5
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumNotesMelody5 { 1 };
	
	//TODO: Add an Enum what Instrument the Enemy is weak to

	//Player Class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> PlayerClass { nullptr };
	
	//Enemy Class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> EnemyClass { nullptr };

	//Enemy Skeletal Mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* EnemySkeletalMesh { nullptr };
	
};
