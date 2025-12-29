// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Quartz/AudioMixerClockHandle.h"
#include "TestEnemyActor.generated.h"

class UTimelineComponent;
class UCurveFloat;
class AGameHUD;
class ANodeActor;
class USplineComponent;

UCLASS()
class MASTERRHYTHMGAME_API ATestEnemyActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestEnemyActor();

	UFUNCTION(BlueprintCallable)
	int32 CalcHealth1(int32 Value);

	UFUNCTION(BlueprintCallable)
	int32 CalcHealth2(int32 Value);

	UFUNCTION(BlueprintCallable)
	int32 CalcHealth3(int32 Value);

	UFUNCTION(BlueprintCallable)
	int32 CalcHealth4(int32 Value);

	UFUNCTION(BlueprintCallable)
	int32 CalcHealth5(int32 Value);

	UFUNCTION(BlueprintCallable)
	int32 CalcHealth6(int32 Value);

	UFUNCTION(BlueprintCallable)
	int32 CalcHealth7(int32 Value);

	UFUNCTION(BlueprintCallable)
	int32 CalcHealth8(int32 Value);

	UFUNCTION()
	void ApplyDamage(int32 DamageValue);

	UFUNCTION()
	void CreateAndStartQuartzClock(int32 InBPM);

	// Timeline helpers (setup a single-frame timeline to initialize UI one frame after BeginPlay)
	void SetupHealthTimeline();

	UFUNCTION()
	void OnHealthTimelineTick(float Value);

	UFUNCTION()
	void OnHealthTimelineFinished();

	// --- SplineRef accessors ---
	USplineComponent* GetSplineRef() const { return SplineRef; }
	void SetSplineRef(USplineComponent* InSpline) { SplineRef = InSpline; }

	// --- BPM ---
	int32 GetBPM() const { return BPM; }
	void SetBPM(int32 InBPM) { BPM = InBPM; }

	// --- HealthBar accessors ---
	int32 GetHealth1() const { return HealthBar1; }
		void SetHealth1(int32 InHealth) { HealthBar1 = InHealth; }

	int32 GetHealth2() const { return HealthBar2; }
	void SetHealth2(int32 InHealth) { HealthBar2 = InHealth; }

	int32 GetHealth3() const { return HealthBar3; }
	void SetHealth3(int32 InHealth) { HealthBar3 = InHealth; }

	int32 GetHealth4() const { return HealthBar4; }
	void SetHealth4(int32 InHealth) { HealthBar4 = InHealth; }

	int32 GetHealth5() const { return HealthBar5; }
	void SetHealth5(int32 InHealth) { HealthBar5 = InHealth; }

	int32 GetHealth6() const { return HealthBar6; }
	void SetHealth6(int32 InHealth) { HealthBar6 = InHealth; }

	int32 GetHealth7() const { return HealthBar7; }
	void SetHealth7(int32 InHealth) { HealthBar7 = InHealth; }

	int32 GetHealth8() const { return HealthBar8; }
	void SetHealth8(int32 InHealth) { HealthBar8 = InHealth; }

	UFUNCTION()
	void Attack(int32 InBPM);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAudioComponent> AudioComponent { nullptr };

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Scene { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "DefaultSceneRoot", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> DefaultSceneRoot { nullptr };

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> Mesh { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 HealthBar1 { 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 HealthBar2 { 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 HealthBar3 { 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 HealthBar4 { 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 HealthBar5 { 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 HealthBar6 { 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 HealthBar7 { 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 HealthBar8 { 0 };

	UPROPERTY()
	TObjectPtr<USplineComponent> SplineRef { nullptr };

	UPROPERTY()
	TObjectPtr<UQuartzClockHandle> QuartzClockHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 BPM { 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<ANodeActor> NodeActor { nullptr };

	UPROPERTY()
	TObjectPtr<AGameHUD> GameHUD { nullptr };

	UPROPERTY()
	TObjectPtr<UTimelineComponent> HealthTimeline { nullptr };

	// Keep a runtime-created curve so GC won't collect it
	UPROPERTY()
	TObjectPtr<UCurveFloat> HealthCurve { nullptr };
};