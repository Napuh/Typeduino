/*
  ************************************************************||
  ||  Copyright (c) 2022 Napuh (Licensed under MIT)           ||
  ||  Based on open source libraries from the Arduino Project ||
  ||  and the open source proyect Malduino from Seytonic.     ||
  ************************************************************||
*/
#include <Arduino.h>
#include <utils.h>
#include <display_errors.h>

/* Use for debugging ONLY */
#define debug

/* Standard libraries */
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <string.h>

/* Keyboard dependent libraries */
#include <Keyboard.h>
// #include <Keyboard_ES.h>

/*
    IR library developed by NicoHood
    Github: https://github.com/NicoHood/IRLremote
*/
#include "IRLremote.h"

/* Button IR code definitions */

#define BUTTON1 0x74B2F0AD
#define BUTTON2 0x85F2EAD
#define BUTTON3 0xA77BB6AD
#define BUTTON4 0x40BF10EF
#define BUTTON5 0x40BF50AF
#define BUTTON6 0x40BFB04F

/* Defining constants */
int defaultDelay = 5;
int defaultCharDelay = 5;

/* Defining arduino pins */
const int CSPinSDCard = 10;

/* Defining IR variables */
const int pinIR = 2;
CHashIR IRLremote;

/* Defining useful global variables */
char *repeatBuffer = malloc(sizeof(char) * 12);
int bufSize = 0;
File script;
String scriptName;

/* Defining special keys */
#define KEYPAD_0 234
#define KEYPAD_1 225
#define KEYPAD_2 226
#define KEYPAD_3 227
#define KEYPAD_4 228
#define KEYPAD_5 229
#define KEYPAD_6 230
#define KEYPAD_7 231
#define KEYPAD_8 232
#define KEYPAD_9 233
#define KEYPAD_ASTERIX 221
#define KEYPAD_ENTER 224
#define KEYPAD_MINUS 222
#define KEYPAD_PERIOD 235
#define KEYPAD_PLUS 223
#define KEYPAD_SLASH 220
#define PRINTSCREEN 206

/* Defining functions */
void setup();
void loop();
String getDipSwitchState();
void FatalError();
void CommonError();
void turnOnLEDS();
void turnOffLEDS();
int getSpace(int start, int end);
bool equals(char *strA, int start, int end, char *strB, int strLen);
int toPositive(int num);
bool equalsBuffer(int start, int end, char *str);
int getInt(char *str, int pos);
void KeyboardWrite(uint8_t c);
void runLine();
void runCommand(int s, int e);

void KeyboardWrite(uint8_t c)
{
    Keyboard.press(c);
    delay(defaultCharDelay);
    Keyboard.release(c);
}

void runLine()
{
    int space = getSpace(0, bufSize);
    if (space == -1)
        runCommand(0, bufSize);
    else
    {
        if (equalsBuffer(0, space, "DEFAULTDELAY") || equalsBuffer(0, space, "DEFAULT_DELAY"))
            defaultDelay = getInt(buf, space);
        else if (equalsBuffer(0, space, "DEFAULTCHARDELAY") || equalsBuffer(0, space, "DEFAULT_CHAR_DELAY"))
            defaultCharDelay = getInt(buf, space);
        else if (equalsBuffer(0, space, "DELAY"))
        {
            delay(getInt(buf, space));
        }
        else if (equalsBuffer(0, space, "STRING"))
        {
            for (int i = space + 1; i < bufSize; i++)
                KeyboardWrite(buf[i]);
        }
        else if (equalsBuffer(0, space, "RANDOMMIN"))
            rMin = getInt(buf, space);
        else if (equalsBuffer(0, space, "RANDOMMAX"))
            rMax = getInt(buf, space);
        else if (equalsBuffer(0, space, "REM") || equalsBuffer(0, space, "REPEAT"))
        {
        }
        else
        {
            runCommand(0, space);
            while (space >= 0 && space < bufSize)
            {
                int nSpace = getSpace(space + 1, bufSize);
                if (nSpace == -1)
                    nSpace = bufSize;
                runCommand(space + 1, nSpace);
                space = nSpace;
            }
        }
    }

    Keyboard.releaseAll();
    delay(defaultDelay);
}

void runCommand(int s, int e)
{

    if (e - s < 2)
        Keyboard.press(buf[s]);
    else if (equalsBuffer(s, e, "ENTER"))
        Keyboard.press(KEY_RETURN);
    else if (equalsBuffer(s, e, "GUI") || equalsBuffer(s, e, "WINDOWS"))
        Keyboard.press(KEY_LEFT_GUI);
    else if (equalsBuffer(s, e, "SHIFT"))
        Keyboard.press(KEY_LEFT_SHIFT);
    else if (equalsBuffer(s, e, "ALT") || equalsBuffer(s, e, "ALT_LEFT") || equalsBuffer(s, e, "ALTLEFT"))
        Keyboard.press(KEY_LEFT_ALT);
    else if (equalsBuffer(s, e, "ALT_RIGHT") || equalsBuffer(s, e, "ALTRIGHT"))
        Keyboard.press(KEY_RIGHT_ALT);
    else if (equalsBuffer(s, e, "CTRL") || equalsBuffer(s, e, "CONTROL"))
        Keyboard.press(KEY_LEFT_CTRL);
    else if (equalsBuffer(s, e, "CAPSLOCK"))
        Keyboard.press(KEY_CAPS_LOCK);
    else if (equalsBuffer(s, e, "DELETE"))
        Keyboard.press(KEY_DELETE);
    else if (equalsBuffer(s, e, "END"))
        Keyboard.press(KEY_END);
    else if (equalsBuffer(s, e, "ESC") || equalsBuffer(s, e, "ESCAPE"))
        Keyboard.press(KEY_ESC);
    else if (equalsBuffer(s, e, "HOME"))
        Keyboard.press(KEY_HOME);
    else if (equalsBuffer(s, e, "INSERT"))
        Keyboard.press(KEY_INSERT);
    else if (equalsBuffer(s, e, "PAGEUP"))
        Keyboard.press(KEY_PAGE_UP);
    else if (equalsBuffer(s, e, "PAGEDOWN"))
        Keyboard.press(KEY_PAGE_DOWN);
    else if (equalsBuffer(s, e, "SPACE"))
        Keyboard.press(' ');
    else if (equalsBuffer(s, e, "TAB"))
        Keyboard.press(KEY_TAB);
    else if (equalsBuffer(s, e, "BACKSPACE"))
        Keyboard.press(KEY_BACKSPACE);
    else if (equalsBuffer(s, e, "UP") || equalsBuffer(s, e, "UPARROW"))
        Keyboard.press(KEY_UP_ARROW);
    else if (equalsBuffer(s, e, "DOWN") || equalsBuffer(s, e, "DOWNARROW"))
        Keyboard.press(KEY_DOWN_ARROW);
    else if (equalsBuffer(s, e, "LEFT") || equalsBuffer(s, e, "LEFTARROW"))
        Keyboard.press(KEY_LEFT_ARROW);
    else if (equalsBuffer(s, e, "RIGHT") || equalsBuffer(s, e, "RIGHTARROW"))
        Keyboard.press(KEY_RIGHT_ARROW);
    else if (equalsBuffer(s, e, "PRINTSCREEN"))
        Keyboard.press(PRINTSCREEN);
    else if (equalsBuffer(s, e, "F1"))
        Keyboard.press(KEY_F1);
    else if (equalsBuffer(s, e, "F2"))
        Keyboard.press(KEY_F2);
    else if (equalsBuffer(s, e, "F3"))
        Keyboard.press(KEY_F3);
    else if (equalsBuffer(s, e, "F4"))
        Keyboard.press(KEY_F4);
    else if (equalsBuffer(s, e, "F5"))
        Keyboard.press(KEY_F5);
    else if (equalsBuffer(s, e, "F6"))
        Keyboard.press(KEY_F6);
    else if (equalsBuffer(s, e, "F7"))
        Keyboard.press(KEY_F7);
    else if (equalsBuffer(s, e, "F8"))
        Keyboard.press(KEY_F8);
    else if (equalsBuffer(s, e, "F9"))
        Keyboard.press(KEY_F9);
    else if (equalsBuffer(s, e, "F10"))
        Keyboard.press(KEY_F10);
    else if (equalsBuffer(s, e, "F11"))
        Keyboard.press(KEY_F11);
    else if (equalsBuffer(s, e, "F12"))
        Keyboard.press(KEY_F12);
    else if (equalsBuffer(s, e, "NUM_0"))
        KeyboardWrite(KEYPAD_0);
    else if (equalsBuffer(s, e, "NUM_1"))
        KeyboardWrite(KEYPAD_1);
    else if (equalsBuffer(s, e, "NUM_2"))
        KeyboardWrite(KEYPAD_2);
    else if (equalsBuffer(s, e, "NUM_3"))
        KeyboardWrite(KEYPAD_3);
    else if (equalsBuffer(s, e, "NUM_4"))
        KeyboardWrite(KEYPAD_4);
    else if (equalsBuffer(s, e, "NUM_5"))
        KeyboardWrite(KEYPAD_5);
    else if (equalsBuffer(s, e, "NUM_6"))
        KeyboardWrite(KEYPAD_6);
    else if (equalsBuffer(s, e, "NUM_7"))
        KeyboardWrite(KEYPAD_7);
    else if (equalsBuffer(s, e, "NUM_8"))
        KeyboardWrite(KEYPAD_8);
    else if (equalsBuffer(s, e, "NUM_9"))
        KeyboardWrite(KEYPAD_9);
    else if (equalsBuffer(s, e, "NUM_ASTERIX"))
        KeyboardWrite(KEYPAD_ASTERIX);
    else if (equalsBuffer(s, e, "NUM_ENTER"))
        KeyboardWrite(KEYPAD_ENTER);
    else if (equalsBuffer(s, e, "NUM_Minus"))
        KeyboardWrite(KEYPAD_MINUS);
    else if (equalsBuffer(s, e, "NUM_PERIOD"))
        KeyboardWrite(KEYPAD_PERIOD);
    else if (equalsBuffer(s, e, "NUM_PLUS"))
        KeyboardWrite(KEYPAD_PLUS);
}

void setup()
{
#ifdef debug
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
    Serial.println("Typeduino initialized");
#endif
    pinMode(RXLED, OUTPUT);
    IRLremote.begin(pinIR);
}

void loop()
{
    if (IRLremote.available())
    {
        scriptName = "";
        auto data = IRLremote.read();

        switch (data.command)
        {
        case BUTTON1:
            scriptName = "01";
            break;
        case BUTTON2:
            scriptName = "02";
            break;
        case BUTTON3:
            scriptName = "03";
            break;
        case BUTTON4:
            scriptName = "04";
            break;
        case BUTTON5:
            scriptName = "05";
            break;
        case BUTTON6:
            scriptName = "06";
            break;
        default:
            Serial.println("BUtton not identified");
            break;
        }

        if (scriptName != "")
        {
            scriptName += ".txt";
#ifdef debug
            Serial.println("executing script " + scriptName);
#endif
            if (!SD.begin(SPI_HALF_SPEED, CSPinSDCard))
            {

#ifdef debug
                Serial.println("Error when openning SD Card");
#endif
                FatalError();
            }
            script = SD.open(scriptName);
            if (!script)
            {
                CommonError();
#ifdef debug
                Serial.println("Error when openning script");
                Serial.println("Script name: " + scriptName);
                Serial.println("This error can be caused because the script");
                Serial.println("does not exist, double check that");
#endif
            }
            else
            {
#ifdef debug
                Serial.println("Sucess accessing script");
#endif
                turnOnLEDS();
                while (script.available())
                {
                    buf[bufSize] = script.read();
                    if (buf[bufSize] == '\r' || buf[bufSize] == '\n' || bufSize >= buffersize)
                    {
                        if (buf[bufSize] == '\r' && script.peek() == '\n')
                            script.read();

                        int repeatBufferSize = 0;
                        int repeats = 0;
                        unsigned long payloadPosition = script.position();

                        for (int i = 0; i < 12; i++)
                        {
                            if (script.available())
                            {
                                repeatBuffer[repeatBufferSize] = script.read();
                                repeatBufferSize++;
                            }
                            else
                                break;
                        }

                        if (repeatBufferSize > 6)
                        {
                            if (equals(repeatBuffer, 0, 6, "REPEAT", 6))
                            {
                                repeats = getInt(repeatBuffer, 6);
                            }
                        }

                        for (int i = 0; i < repeats; i++)
                            runLine();

                        script.seek(payloadPosition);
                        runLine();
                        bufSize = 0;
                    }
                    else
                        bufSize++;
                }

                if (bufSize > 0)
                {
                    runLine();
                    bufSize = 0;
                }
                script.close();
                turnOffLEDS();
            }
#ifdef debug
            Serial.println("Sucess executing script");
#endif
            delay(200);
        }
    }
}