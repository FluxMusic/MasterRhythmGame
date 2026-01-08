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

UENUM(BlueprintType)
enum class EWorldMapItem : uint8
{
	LevelOne = 0,
	LevelTwo = 1,
	LevelThree = 2,
	LevelFour = 3,
	LevelFive = 4,
	LevelSix = 5,
	MainMenu = 6
};

UENUM(BlueprintType)
enum class EControllerStateWorldMap : uint8
{
	WorldMap,
	PauseMenu,
	AudioMenu,
	GraphicsMenu,
	SettingMenu
};

UENUM(BlueprintType)
enum class EPauseMenuItem : uint8
{
	Resume = 0,
	Save = 1,
	Load = 2,
	Settings = 3,
	MainMenu = 4,
	Quit = 5
};

UENUM(BlueprintType)
enum class EMainMenuItem : uint8
{
	NewGame = 0,
	LoadGame = 1,
	Setting = 2,
	Credits = 3,
	Escape = 4
};

UENUM(BlueprintType)
enum class ELoadGameMenuItem : uint8
{
	FirstLevel = 0,
	SecondLevel = 1,
	ThirdLevel = 2,
	MainMenu = 3
};

UENUM(BlueprintType)
enum class EMainSettingItem : uint8
{
	Graphic = 0,
	Audio = 1,
	MainMenu = 2
};

UENUM(BlueprintType)
enum class EGraphicSettingItem : uint8
{
	WindowModeDown = 0,
	WindowModeUp = 1,
	ResolutionDown = 2,
	ResolutionUp = 3,
	GraphicDown = 4,
	GraphicUp = 5,
	VSyncDown = 6,
	VSyncUp = 7,
	Apply = 8,
	Return = 9
};

UENUM(BlueprintType)
enum class EAudioSettingItem : uint8
{
	MasterVolumeSlider = 0,
	MusicVolumeSlider = 1,
	SfxVolumeSlider = 2,
	Return = 3
};

UENUM(BlueprintType)
enum class EControllerState : uint8
{
	MainMenu,
	CreditsMenu,
	AudioMenu,
	GraphicsMenu,
	LoadMenu,
	SettingMenu
};

UENUM(BlueprintType)
enum class EControllerStateGame : uint8
{
	PauseMenu,
	Game,
	SettingMenu,
	GraphicsMenu,
	AudioMenu,
	DeathMenu
};

UENUM(BlueprintType)
enum class EDeathState : uint8
{
	Respawn,
	WorldMap,
	MainMenu
};