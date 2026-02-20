// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CoreTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimerManager.h"
#include "AudioManagerSubsystem.generated.h"

class AGameHUD;
class ASplineActor;
class ATestEnemyActor;
class ANodeActor;
class UQuartzSubsystem;
class UQuartzClockHandle;
class ULevelData;
struct FMetaSoundOutput;

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UAudioManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
    UAudioManagerSubsystem();

    UFUNCTION()
    void InitSubsystem(ULevelData* Data);

    // --- Quartz control ---
    UFUNCTION(BlueprintCallable)
    void StartClock();

    UFUNCTION(BlueprintCallable)
    void StopClock();

    UFUNCTION()
    void ClockHandleInit(FName ClockName);

    UFUNCTION()
    void SetBeatsPerMinute(float InBPM, FQuartzQuantizationBoundary InBoundary, FOnQuartzCommandEventBP InDelegate);

    UFUNCTION()
    void PlayQuantized(UAudioComponent* AudioComponent);

	// --- Enemy Can Attack accessors ---
	bool GetEnemyCanAttack() const { return bEnemyCanAttack; }

	// --- Part Name Fix accessors ---
	FString GetPartNameFix() const { return PartNameFix; }

    //Get Beat Seconds used for Input Latency Test
    double GetBeatSeconds() const { return LastBeatAudioTimeSeconds; }
    
    //Gets the Time when the Output of the Metasound is read used for Latency Test
    double GetMetaSoundOutputTimeSeconds() const { return LastBeatAudioTimeSeconds; }

    double GetLatencyBetweenThreads() const { return LatencyBetweenThreads; }

    //Get the Subsystem, used for Input Latency Test
    UQuartzSubsystem* GetQuartzSubsystem() { return QuartzSubsystem; }

    UFUNCTION(BlueprintCallable)
    float GetAnimTime();

    // --- Score System ---
    UFUNCTION(BlueprintCallable)
    int32 GetCurrentScore() const { return CurrentScore; }

    UFUNCTION(BlueprintCallable)
    float GetCurrentComboMultiplier() const { return CurrentComboMultiplier; }

    UFUNCTION(BlueprintCallable)
    int32 GetCurrentComboCount() const { return CurrentComboCount; }

    UFUNCTION(BlueprintCallable)
    int32 GetBaseScorePerNote() const { return BaseScorePerNote; }

    UFUNCTION()
    void AddScore(int32 BasePoints);

    UFUNCTION()
    void ResetCombo();

    UFUNCTION()
    void SetScoringEnabled(bool bEnabled);

private:

    UFUNCTION()
	void WatchOutputMidiNoteChange(FName OutputName, const FMetaSoundOutput& Output);

    UFUNCTION()
    void WatchOutputPartFinishedPart(FName OutputName, const FMetaSoundOutput& Output);

    UFUNCTION()
    void WatchOutputPartFinishedName(FName OutputName, const FMetaSoundOutput& Output);

    UFUNCTION()
    void WatchOutputPartFinishedPercent(FName OutputName, const FMetaSoundOutput& Output);

    UFUNCTION()
    void StartIntro();

    UFUNCTION()
    void StartPart1Intro();

    UFUNCTION()
    void StartPart1();

    UFUNCTION()
    void StartPart2Intro();

    UFUNCTION()
    void StartPart2();

    UFUNCTION()
    void StartPart3Intro();

    UFUNCTION()
    void StartPart3();

    UFUNCTION()
    void StartPart4Intro();

    UFUNCTION()
	void StartPart4();

    UFUNCTION()
    void StartPart5Intro();

    UFUNCTION()
    void StartPart5();

    UFUNCTION()
    void StartOutro();

    UFUNCTION()
    void StartOutroLoop();

    UFUNCTION()
    void OnQuartzClockBeat(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction);

    UFUNCTION()
    void StartMusic();

    UFUNCTION()
	void MuteLeads();
    
    UFUNCTION()
	void UnmuteLeads();

    UFUNCTION()
    void HandleSwampLevel();

    UFUNCTION()
    void HandleVolcanoLevel();

    UFUNCTION()
    void HandleSpaceLevel();

    UFUNCTION()
    void HandleIceLevel();

    UFUNCTION()
    void HandleCyberpunkLevel();

    UFUNCTION()
    void HandleSteampunkLevel();

    UFUNCTION()
    void HandleTestLevel();

    UFUNCTION()
    void HandlePart1(bool LoadOtherLevel, FName UnloadLevelName = NAME_None, FName LoadLevelName = NAME_None);

    UFUNCTION()
    void HandlePart2(bool LoadOtherLevel, FName UnloadLevelName = NAME_None, FName LoadLevelName = NAME_None);

    UFUNCTION()
    void HandlePart3(bool LoadOtherLevel, FName UnloadLevelName = NAME_None, FName LoadLevelName = NAME_None);

    UFUNCTION()
	void HandlePart4(bool LoadOtherLevel, FName UnloadLevelName = NAME_None, FName LoadLevelName = NAME_None);

    UFUNCTION()
    void HandlePart5();

    UFUNCTION()
    void HandleOutro();

    UFUNCTION()
    EMapNames GetMapTypeFromString(const FString& MapName);

private:

    UPROPERTY()
    UQuartzSubsystem* QuartzSubsystem;

    UPROPERTY()
    UQuartzClockHandle* ClockHandle;

    UPROPERTY()
    int32 NumBarsDelay { 1 };

    // The currently active audio component (set when PlayQuantized is called)
    UPROPERTY()
    UAudioComponent* ActiveAudioComponent{ nullptr };

	UPROPERTY()
    TSubclassOf<ATestEnemyActor> EnemyClass { nullptr };

    UPROPERTY()
    TObjectPtr<ATestEnemyActor> Enemy { nullptr };

    UPROPERTY()
	TObjectPtr<ASplineActor> Spline { nullptr };

    UPROPERTY()
    bool bEnemyCanAttack { true };

    UPROPERTY()
    bool bPlayerCanCollectNotes { true };

    UPROPERTY()
    FString PartNameFix;

    FString PartPlayed;

    UPROPERTY()
    bool bPartAlreadyPlayed { false };

    // New: track whether Part1IntroEnd has occurred at least once
    UPROPERTY()
    bool bPart1IntroEnded { false };

    UPROPERTY()
    TObjectPtr<AGameHUD> GameHUD { nullptr };

    UPROPERTY()
    ENote RootNote { ENote::C };

    UPROPERTY()
    EPartFinish PartFinish { EPartFinish::Three };

    // Score System
    UPROPERTY()
    int32 CurrentScore { 0 };

    UPROPERTY()
    float CurrentComboMultiplier { 1.0f };

    UPROPERTY()
    int32 CurrentComboCount { 0 };

    UPROPERTY()
    bool bScoringEnabled { true };

    UPROPERTY()
    int32 BaseScorePerNote { 50 };

    UPROPERTY()
    float ComboMultiplierMin { 1.0f };

    UPROPERTY()
    float ComboMultiplierMax { 5.0f };

    UPROPERTY()
    float ComboMultiplierIncrement { 0.1f };

    //Animation Stuff

    //Animation has a small startup, this is used to calculate that (should be 0.17f but somehow that doesn't work)
    // const float AnimStartupTime { 0.17f };
    const float AnimStartupTime { 0.f };
    //The Length of the idle Animation
    const float AnimLength { 1.f };
    //The Amount of bars the anim loops over
    const float LoopLength { 2.f };

    //The explicit Time of the animation, used to sync the idle anim to the BPM
    float AnimTime { 0.f };



    //Test for Input Latency
    double LastBeatAudioTimeSeconds { 0.0 };
    
    //The Time when the MetaSound Output is read
    double MetaSoundOutputTimeSeconds { 0.0 };

    double LatencyBetweenThreads { 0.0 };
};