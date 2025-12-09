// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodeActor.generated.h"

class USplineComponent;
class UTimelineComponent;

UCLASS()
class MASTERRHYTHMGAME_API ANodeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANodeActor();

	UFUNCTION()
	void OnNoteBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void MoveLeft();

	UFUNCTION()
	void MoveRight();

	UFUNCTION()
	void SetBarLength(double BPM);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", DisplayName = "Mesh")
	TObjectPtr<UStaticMeshComponent> NoteMesh { nullptr };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> Note;

	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneRoot;



	UPROPERTY()
	TObjectPtr<UTimelineComponent> Timeline;

	UPROPERTY()
	TObjectPtr<USplineComponent> SplineRef;

	UPROPERTY()
	bool bIsSentBack;
};
