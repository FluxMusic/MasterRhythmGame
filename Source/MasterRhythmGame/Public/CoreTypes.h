#pragma once

#include "CoreTypes.generated.h"


UENUM(BlueprintType)
enum class EWorld : uint8
{
    Swamp     UMETA(DisplayName="Swamp"),
    Cyberpunk UMETA(DisplayName="Cyberpunk"),
    Steampunk UMETA(DisplayName="Steampunk"),
    Space     UMETA(DisplayName="Space"),
    Ice       UMETA(DisplayName="Ice"),
    Volcano   UMETA(DisplayName="Volcano"),
};

UENUM(BlueprintType)
enum class ENote : uint8
{
	C      = 0,
	CSharp = 1,
	D      = 2,
	DSharp = 3,
	E      = 4,
	F      = 5,
	FSharp = 6,
	G      = 7,
	GSharp = 8,
	A      = 9,
	ASharp = 10,
	B      = 11
};

USTRUCT(BlueprintType)
struct FScale
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> Scale;
};