#include <Arduino.h>
const int RXLED = 17;

void FatalError()
{
  while (1)
  {
    digitalWrite(RXLED, HIGH);
    delay(50);
    TXLED0;
    delay(50);
    digitalWrite(RXLED, LOW);
    delay(50);
    TXLED1;
    delay(50);
  }
}

void CommonError()
{
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(RXLED, HIGH);
    delay(200);
    TXLED0;
    delay(200);
    digitalWrite(RXLED, LOW);
    delay(200);
    TXLED1;
    delay(200);
  }
}

void turnOnLEDS()
{
  digitalWrite(RXLED, HIGH);
  TXLED1;
}

void turnOffLEDS()
{
  digitalWrite(RXLED, LOW);
  TXLED0;
}