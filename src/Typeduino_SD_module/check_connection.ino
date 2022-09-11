#include <SPI.h>
#include <SD.h>
#include <string.h>

Sd2Card card;
SdVolume volume;
SdFile root;
const int buttonPin = 2;

const int dipSwitch1Pin = 6;
const int dipSwitch2Pin = 7;
const int dipSwitch3Pin = 8;
const int dipSwitch4Pin = 9;

int dipSwitch1State = 0;
int dipSwitch2State = 0;
int dipSwitch3State = 0;
int dipSwitch4State = 0;

String state = "0000";

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
    pinMode(buttonPin, INPUT);
    pinMode(dipSwitch1Pin, INPUT_PULLUP);
    pinMode(dipSwitch2Pin, INPUT_PULLUP);
    pinMode(dipSwitch3Pin, INPUT_PULLUP);
    pinMode(dipSwitch4Pin, INPUT_PULLUP);
    Serial.println("Typeduino initialized");
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
}

void loop()
{
    /*
       Button check
    */
    if (digitalRead(buttonPin) == HIGH)
    {
        Serial.println("Button pressed");
    }

    /*
       Dip switch test
    */
    dipSwitch1State = digitalRead(dipSwitch1Pin);
    dipSwitch2State = digitalRead(dipSwitch2Pin);
    dipSwitch3State = digitalRead(dipSwitch3Pin);
    dipSwitch4State = digitalRead(dipSwitch4Pin);

    if (digitalRead(dipSwitch1Pin) == LOW)
    {
        state[0] = '1';
    }
    else
    {
        state[0] = '0';
    }

    if (digitalRead(dipSwitch2Pin) == LOW)
    {
        state[1] = '1';
    }
    else
    {
        state[1] = '0';
    }

    if (digitalRead(dipSwitch3Pin) == LOW)
    {
        state[2] = '1';
    }
    else
    {
        state[2] = '0';
    }

    if (digitalRead(dipSwitch4Pin) == LOW)
    {
        state[3] = '1';
    }
    else
    {
        state[3] = '0';
    }

    Serial.println("Current dip switch state is: " + state);
