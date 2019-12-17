#include <avr/sleep.h>

// disable ADC (before power-off)
#define adc_disable() (ADCSRA &= ~(1<<ADEN)) 

const int ledPin = 0;
const int buttonPin = 2;

unsigned long nowTime = 0;
unsigned long oldTime = 0;

// 4 hours (4h*60min*60sec*1000ms = 14400000)
unsigned long onTime = 14400000;

// state to keep track of led
bool ledState = false;
bool buttonState = false;

void setup() 
{
  // disabiling to conserve power
  adc_disable();

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // setting as outputs to conserve power
  pinMode(1, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
}

void ledOn()
{
  ledState = true;
  digitalWrite(ledPin, ledState);
}

void ledOff()
{
  ledState = false;
  digitalWrite(ledPin, ledState);
}

void sleep()
{
  sleep_enable();
  sleep_cpu();
}

void loop() 
{
  nowTime = millis();
  buttonState = !digitalRead(buttonPin);

  // if time is less than 4 hours
  if (nowTime < onTime)
  {
    // if led is off and less than 10 ms
    if (!ledState && nowTime - oldTime < 10)
    {
      ledOn();
    }
    // if led is on and grater than 10 ms
    else if (ledState && nowTime - oldTime > 10)
    {
      ledOff();
      
      // set to idle sleep mode
      set_sleep_mode(SLEEP_MODE_IDLE);
      sleep();
    }
    // if time is less than 2 sec
    else if (nowTime - oldTime > 2010)
    {
      oldTime = nowTime;
    }
  }
  else
  {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep();
  }

  // if button was pressed turn off prematuraly
  if (buttonState)
  {
    // incase led is set on at time of press
    ledOff();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep();
  }
}