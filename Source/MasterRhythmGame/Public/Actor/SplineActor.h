// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../CoreTypes.h"

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
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	USplineComponent* GetSplines(int32 Int);

	UFUNCTION(BlueprintCallable)
	FVector GetSplinesY(int32 Int);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "DefaultSceneRoot") 
	TObjectPtr<USceneComponent> DefaultSceneRoot { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "SplineB")
	TObjectPtr<USplineComponent> SplineB      { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "SplineASharp")
	TObjectPtr<USplineComponent> SplineASharp { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "SplineA")
	TObjectPtr<USplineComponent> SplineA      { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "SplineGSharp")
	TObjectPtr<USplineComponent> SplineGSharp { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "SplineG")
	TObjectPtr<USplineComponent> SplineG      { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "SplineFSharp")
	TObjectPtr<USplineComponent> SplineFSharp { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "SplineF")
	TObjectPtr<USplineComponent> SplineF      { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "SplineE")
	TObjectPtr<USplineComponent> SplineE      { nullptr };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "SplineDSharp")
	TObjectPtr<USplineComponent> SplineDSharp { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "SplineD")
	TObjectPtr<USplineComponent> SplineD      { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<USplineComponent> SplineCSharp { nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", DisplayName = "SplineC")
	TObjectPtr<USplineComponent> SplineC      { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float LineSpacing { 75.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", DisplayName = "SplineLength")
	double SplineLength { 2000.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<UInstancedStaticMeshComponent> MeshInstances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<UMaterialInterface> Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", DisplayName = "Scale")
	EWorld Scale { EWorld::Swamp };

private:
	//Lookup Table to remap the Notes to the scale of the World's Theme
	UPROPERTY()
    TMap<EWorld, FScale> ScaleLookupTable
    {
        { EWorld::Swamp,     { { 0, 2, 4, 5, 7, 9, 11 } } }, // C Major
        { EWorld::Cyberpunk, { { 0, 2, 3, 5, 7, 9, 10 } } }, // G Aeolian
        { EWorld::Steampunk, { { 0, 2, 3, 5, 7, 8, 10 } } }, // F Dorian
        { EWorld::Space,     { { 1, 2, 4, 6, 7, 9, 11 } } }, // E Dorian
        { EWorld::Ice,       { { 0, 1, 3, 5, 7, 8, 10 } } }, // F Aeolian
        { EWorld::Volcano,   { { 0, 2, 4, 5, 7, 9, 11 } } }  // A Minor
    };
};
