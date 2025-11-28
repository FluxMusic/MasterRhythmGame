// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Quartz/AudioMixerClockHandle.h"
#include "TestEnemyActor.generated.h"

class USplineComponent;

UCLASS()
class MASTERRHYTHMGAME_API ATestEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestEnemyActor();

	UFUNCTION(BlueprintCallable)
	int32 CalcHealth(int32 Value, int32 OutputValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> Scene { nullptr };

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> Mesh { nullptr };

	UPROPERTY()
	int32 Health { 0 };

	UPROPERTY()
	TObjectPtr<USplineComponent> SplineRef { nullptr };

	//UPROPERTY()
	//TObjectPtr<UQuartzClockHandle> QuartzClockHandle;
	
};
