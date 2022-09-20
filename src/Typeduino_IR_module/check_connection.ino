/*
  ************************************************************
  ||  Copyright (c) 2022 Napuh (Licensed under MIT)         ||
  ************************************************************
*/

#include "IRLremote.h"
#include <SPI.h>
#include <SD.h>
#include <string.h>

Sd2Card card;
SdVolume volume;
SdFile root;

const int pinIR = 2;
CHashIR IRLremote;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
    IRLremote.begin(pinIR);
    Serial.println(("Typeduino initialized"));

    /*
       SD Card module check
    */
    Serial.print("\nChecking SD Card connection \n");
    if (!card.init(SPI_HALF_SPEED, 10))
    {
        Serial.println("SD Card detection failed, check again your connection or your SD card format");
        return;
    }
    else
    {
        Serial.println("Wiring is correct and a card is present.");
    }
    Serial.print("\nCard type: ");
    switch (card.type())
    {
    case SD_CARD_TYPE_SD1:
        Serial.println("SD1");
        break;
    case SD_CARD_TYPE_SD2:
        Serial.println("SD2");
        break;
    case SD_CARD_TYPE_SDHC:
        Serial.println("SDHC");
        break;
    default:
        Serial.println("Unknown");
    }

    if (!volume.init(card))
    {
        Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
        return;
    }

    Serial.println("\nFiles found on the card (name, date and size in bytes): ");
    root.openRoot(volume);
    root.ls(LS_R | LS_DATE | LS_SIZE);
    Serial.println("Start IR receiver check");
}

void loop()
{
    /*
       IR receiver check
    */

    if (IRLremote.available())
    {
        auto data = IRLremote.read();

        // Print the protocol data
        Serial.println("You just pressed a button on your remote!");
        Serial.print(F("Address: 0x"));
        Serial.println(data.address, HEX);
        Serial.print(F("Command: 0x"));
        Serial.println(data.command, HEX);
        Serial.println();
        delay(100);
    }
}
