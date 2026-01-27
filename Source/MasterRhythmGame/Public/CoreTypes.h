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

UENUM(BlueprintType)
enum class EScale : uint8
{
	Major            UMETA(DisplayName="Major (Ionian)"),  //Ionian
	Minor            UMETA(DisplayName="Minor (Aeolian)"), //Aeolian
	Phrygian         UMETA(DisplayName="Phrygian"),        //Phrygian
	Lydian           UMETA(DisplayName="Lydian"),          //Lydian
	Mixolydian       UMETA(DisplayName="Mixolydian"),      //Mixolydian
	Dorian           UMETA(DisplayName="Dorian"),          //Dorian
	Locrian          UMETA(DisplayName="Locrian"),         //Locrian
	HarmonicMinor    UMETA(DisplayName="Harmonic Minor"),  //Harmonic Minor
	MelodicMinor     UMETA(DisplayName="Melodic Minor"),   //Melodic Minor
};

USTRUCT(BlueprintType)
struct FScaleDegrees
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> ScaleDegrees;
};

UENUM(BlueprintType)
enum class EInstrument : uint8
{
	None      = 0      UMETA(DisplayName="None"), // Only used for Instrument Weaknesses

	Piano     = 1 << 0 UMETA(DisplayName="Piano"), //Default, always unlocked
	Violin    = 1 << 1 UMETA(DisplayName="Violin"),
	Saxophone = 1 << 2 UMETA(DisplayName="Saxophone"),
	Guitar    = 1 << 3 UMETA(DisplayName="Guitar"),
	Synth     = 1 << 4 UMETA(DisplayName="Synth"),

	All       = Piano | Violin | Saxophone | Guitar | Synth
};
ENUM_CLASS_FLAGS(EInstrument);

namespace InstrumentFlags
{
	FORCEINLINE bool HasFlag(const EInstrument& Enum, const EInstrument& Flag)
	{
		return (Enum & Flag) == Flag;
	}
	FORCEINLINE void SetFlag(EInstrument& Enum, const EInstrument& Flag)
	{
		Enum |= Flag;
	}
}

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

UENUM(BlueprintType)
enum class EDirectionWorldMap : uint8
{
	Forward,
	Backward,
	Right,
	Left
};

UENUM(BlueprintType)
enum class EOctave : uint8
{
	None = 0 UMETA(DisplayName = "None"),

	One = 24 UMETA(DisplayName = "Octave 1"),
	Two = 36 UMETA(DisplayName = "Octave 2"),
	Three = 48 UMETA(DisplayName = "Octave 3"),
	Four = 60 UMETA(DisplayName = "Octave 4"),
	Five = 72 UMETA(DisplayName = "Octave 5"),
	Six = 84 UMETA(DisplayName = "Octave 6")
};

UENUM(BlueprintType)
enum class ELevels : uint8
{
	LevelOne,
	LevelTwo,
	LevelThree,
	LevelFour,
	LevelFive,
	LevelSix
};

UENUM(BlueprintType)
enum class EPartFinish : uint8
{
	Three,
	Four,
	Five
};

UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
	Forward  UMETA(DisplayName = "Forward"),
	Backward UMETA(DisplayName = "Backward"),
	Up       UMETA(DisplayName = "Up"),
	Down     UMETA(DisplayName = "Down"),
	Right    UMETA(DisplayName = "Right"),
	Left     UMETA(DisplayName = "Left")
};

UENUM(BlueprintType)
enum class ESegmentType : uint8
{
	Straight  UMETA(DisplayName = "Straight"),
	Up        UMETA(DisplayName = "Up"),
	Down	  UMETA(DisplayName = "Down")
};

UENUM()
enum class EMapNames : uint8
{
	TestMap   UMETA(DisplayName = "TestMap"),
	Swamp     UMETA(DisplayName = "Lvl_Swamp"),
	Cyberpunk UMETA(DisplayName = "Lvl_Cyberpunk"),
	Ice	      UMETA(DisplayName = "Lvl_Ice"),
	Steampunk UMETA(DisplayName = "Lvl_Steampunk"),
	Space     UMETA(DisplayName = "Lvl_Space"),
	Volcano   UMETA(DisplayName = "Lvl_Volcano")
};