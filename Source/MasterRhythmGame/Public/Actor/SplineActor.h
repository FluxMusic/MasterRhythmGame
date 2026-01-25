// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../CoreTypes.h"

#include "GameFramework/Actor.h"
#include "SplineActor.generated.h"

class ULevelData;
class USplineComponent;

UCLASS()
class MASTERRHYTHMGAME_API ASplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineActor();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	USplineComponent* GetSplines(int32 Int);

	UFUNCTION(BlueprintCallable)
	FVector GetSplinesY(int32 Int);

	UFUNCTION()
	TArray<int32> GetScaleDegrees();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "DefaultSceneRoot") 
	TObjectPtr<USceneComponent> DefaultSceneRoot { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "Spline11")
	TObjectPtr<USplineComponent> Spline11 { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "Spline10")
	TObjectPtr<USplineComponent> Spline10 { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "Spline9")
	TObjectPtr<USplineComponent> Spline9  { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "Spline8")
	TObjectPtr<USplineComponent> Spline8  { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "Spline7")
	TObjectPtr<USplineComponent> Spline7  { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "Spline6")
	TObjectPtr<USplineComponent> Spline6  { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "Spline5")
	TObjectPtr<USplineComponent> Spline5  { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "Spline4")
	TObjectPtr<USplineComponent> Spline4  { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "Spline3")
	TObjectPtr<USplineComponent> Spline3  { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "Spline2")
	TObjectPtr<USplineComponent> Spline2  { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "Spline1")
	TObjectPtr<USplineComponent> Spline1  { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "Spline0")
	TObjectPtr<USplineComponent> Spline0  { nullptr };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float LineSpacing { 75.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", DisplayName = "SplineLength")
	double SplineLength { 2000.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<UInstancedStaticMeshComponent> MeshInstances { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<UStaticMesh> Mesh { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<UMaterialInterface> Material { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<ULevelData> LevelData { nullptr };

private:
	//Lookup Table to remap the Notes to the scale of the World's Theme
	UPROPERTY()
    TMap<EScale, FScaleDegrees> ScaleDegreeLookupTable
    {
        { EScale::Major,         { { 0, 2, 4, 5, 7, 9, 11 } } },
        { EScale::Minor,         { { 0, 2, 3, 5, 7, 8, 10 } } }, 
        { EScale::Phrygian,      { { 0, 1, 3, 5, 7, 8, 10 } } },
        { EScale::Lydian,        { { 0, 2, 4, 6, 7, 9, 11 } } },
        { EScale::Mixolydian,    { { 0, 2, 4, 5, 7, 9, 10 } } },
        { EScale::Dorian,        { { 0, 2, 3, 5, 7, 9, 10 } } },
        { EScale::Locrian,       { { 0, 1, 3, 5, 6, 8, 10 } } },
		{ EScale::HarmonicMinor, { { 0, 2, 3, 5, 7, 8, 11 } } },
		{ EScale::MelodicMinor,  { { 0, 2, 3, 5, 7, 9, 11 } } }
    };
};
