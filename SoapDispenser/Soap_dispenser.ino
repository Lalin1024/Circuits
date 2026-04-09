#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

// Pin definitions
#define IR_PWR   PB2   // Powers the IR sensor
#define IR_OUT   PB1   // IR sensor output signal
#define LED_PIN  PB0   // LED indicator

volatile uint8_t ledTimer = 0;   // Countdown in 500ms steps
volatile bool ledActive = false; // Tracks if LED cycle is running
volatile bool coolDown = false;  // 1s delay after LED OFF

ISR(WDT_vect) {
  // Watchdog interrupt — wakes MCU
}

void setup() {
  // Disable unused peripherals
  ADCSRA &= ~(1 << ADEN);   // Disable ADC
  ACSR  |= (1 << ACD);      // Disable Analog Comparator
  PRR   |= (1 << PRADC);    // Power Reduction ADC
  PRR   |= (1 << PRUSI);    // Power Reduction USI
  PRR   |= (1 << PRTIM0);   // Disable Timer0
  PRR   |= (1 << PRTIM1);   // Disable Timer1

  // Configure pins
  DDRB |= (1 << IR_PWR);    // IR power pin as output
  DDRB |= (1 << LED_PIN);   // LED as output
  DDRB &= ~(1 << IR_OUT);   // IR output as input
  PORTB &= ~(1 << IR_OUT);  // No pull-up on IR_OUT
}

void enterSleep(uint8_t wdt_period) {
  MCUSR = 0;
  WDTCR = (1 << WDCE) | (1 << WDE); // Enable config
  WDTCR = (1 << WDIE) | wdt_period; // Interrupt only

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // Disable BOD for lowest current
  sleep_enable();
  MCUCR |= (1 << BODS) | (1 << BODSE);
  MCUCR = (MCUCR & ~(1 << BODSE)) | (1 << BODS);

  sleep_cpu();    // Enter sleep
  sleep_disable();
}

void checkIR() {
  // Only check sensor if LED cycle is NOT active and not cooling down
  if (!ledActive && !coolDown) {
    // Power ON IR sensor
    PORTB |= (1 << IR_PWR);

    // Wait ~15 ms using watchdog
    enterSleep(WDTO_15MS);

    // Check IR sensor output
    if (!(PINB & (1 << IR_OUT))) {  
      // Hand detected (active LOW)
      ledTimer = 4;          // 6 × 500 ms = 3 sec
      ledActive = true;
      PORTB |= (1 << LED_PIN); // Turn LED ON
    }

    // Power OFF IR sensor
    PORTB &= ~(1 << IR_PWR);
  }
}

void loop() {
  
  checkIR();   // Sample IR only if idle and not in cooldown

  // Handle LED timing
  if (ledActive) {
    if (ledTimer > 0) {
      ledTimer--;
    } else {
      PORTB &= ~(1 << LED_PIN);  // Turn LED OFF
      ledActive = false;
      coolDown = true;           // Start 1s cooldown
      ledTimer = 1;              // 2 × 500 ms = 1 sec
    }
  }

  // Handle cooldown timing
  if (coolDown && !ledActive) {
    if (ledTimer > 0) {
      ledTimer--;
    } else {
      coolDown = false;          // Ready for next detection
    }
  }

  // Sleep for 500 ms
  enterSleep(WDTO_500MS);
}
