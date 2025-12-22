// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineActor.generated.h"

class USplineComponent;

UCLASS()
class MASTERRHYTHMGAME_API ASplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineActor();

	UFUNCTION(BlueprintCallable)
	USplineComponent* GetSplines(int32 Int);

	UFUNCTION(BlueprintCallable)
	FVector GetSplinesY(int32 Int);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "DefaultSceneRoot") 
	TObjectPtr<USceneComponent> DefaultSceneRoot { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "Spline6")
	TObjectPtr<USplineComponent> Spline6 { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<USplineComponent> Spline5 { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<USplineComponent> Spline4 { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<USplineComponent> Spline3 { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<USplineComponent> Spline2 { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<USplineComponent> Spline1 { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<USplineComponent> Spline { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", DisplayName = "MeshLength")
	double MeshLength {  0.0f };
};
