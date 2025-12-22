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
	int32 CalcHealth(int32 Value);

	// --- SplineRef accessors ---
	USplineComponent* GetSplineRef() const { return SplineRef; }
	void SetSplineRef(USplineComponent* InSpline) { SplineRef = InSpline; }

	// --- BPM ---
	int32 GetBPM() const { return BPM; }
	void SetBPM(int32 InBPM) { BPM = InBPM; }

	UFUNCTION()
	void Attack(int32 InBPM);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Scene { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "DefaultSceneRoot", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> DefaultSceneRoot{ nullptr };

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> Mesh { nullptr };

	UPROPERTY()
	int32 Health { 0 };

	UPROPERTY()
	TObjectPtr<USplineComponent> SplineRef { nullptr };

	UPROPERTY()
	TObjectPtr<UQuartzClockHandle> QuartzClockHandle;

	UPROPERTY()
	int32 BPM { 0 };
};
