// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MyGameInstance.h"

void UMyGameInstance::SetInstrumentUnlocked(const EInstrument& Instrument, bool bUnlocked)
{
    // We don't need to do anything if the Instrument is already added or not added
    // based on what we want to do here
    if (InstrumentFlags::HasFlag(UnlockedInstruments, Instrument) == bUnlocked)
    {
        return;
    }
    else
    {
        InstrumentFlags::SetFlag(UnlockedInstruments, Instrument);
    }
}

EInstrument UMyGameInstance::GetUnlockedInstruments()
{
    return UnlockedInstruments;
}