// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WorldMapPlayerCharacter.h"
#include "WorldMap/WorldMapNode.h"
#include "WorldMap/WorldMapPath.h"

// Sets default values
AWorldMapPlayerCharacter::AWorldMapPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AWorldMapPlayerCharacter::TryMoveInDirection(const FVector2D& InputDir)
{
	if (!CurrentNode || TargetNode) return;

    AWorldMapNode* BestNode = nullptr;
    float BestDot = 0.5f;

	// Check all connected nodes
    for (int32 i = 0; i < CurrentNode->GetConnectedNodes().Num(); i++)
    {
        AWorldMapNode* Candidate = CurrentNode->GetConnectedNodes()[i];
        if (!Candidate) continue;

        FVector2D Dir = FVector2D(
            Candidate->GetActorLocation().X - CurrentNode->GetActorLocation().X,
            Candidate->GetActorLocation().Y - CurrentNode->GetActorLocation().Y
        ).GetSafeNormal();

        float Dot = FVector2D::DotProduct(Dir, InputDir);

        if (Dot > BestDot)
        {
            BestDot = Dot;
            BestNode = Candidate;
        }
    }

    if (BestNode)
    {
        TargetNode = BestNode;

		// Find correct path
        for (AWorldMapPath* Path : CurrentNode->GetConnectedPaths())
        {
            if (Path->GetStartNode() == CurrentNode && Path->GetEndNode() == BestNode)
            {
                CurrentPath = Path;
                break;
            }
            if (Path->GetEndNode() == CurrentNode && Path->GetStartNode() == BestNode)
            {
                CurrentPath = Path;
                break;
            }
        }

        Progress = 0.f;
    }
}

// Called when the game starts or when spawned
void AWorldMapPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldMapPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
	if (TargetNode && CurrentPath)
    {
        Progress += DeltaTime * MoveSpeed;

        FVector Pos = CurrentPath->GetLocationAtProgress(Progress);
        FRotator Rot = CurrentPath->GetRotationAtProgress(Progress);

        SetActorLocation(Pos);
        SetActorRotation(Rot);

        if (Progress >= 1.f)
        {
            CurrentNode = TargetNode;
            TargetNode = nullptr;
            CurrentPath = nullptr;
        }
    }
}

// Called to bind functionality to input
void AWorldMapPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

