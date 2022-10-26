#include <stdlib.h>

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Settings
static const TickType_t dim_delay_period = 5000 / portTICK_PERIOD_MS;

// Pins
static const int led_pin = LED_BUILTIN;

// Globals
static TimerHandle_t one_shot_timer = NULL;

/*
  Callbacks
 */
void autoDimmerCallback(TimerHandle_t xTimer) {
  digitalWrite(led_pin, LOW);
}

void readSerial(void *parameters) {

  char c;
  
  // configure pins
  pinMode(led_pin, OUTPUT);

  // Clear whole buffer
  while (1) {
    if (Serial.available() > 0) {
      c = Serial.read();
      Serial.print(c);
      digitalWrite(led_pin, HIGH);

      // Start timer
      xTimerStart(one_shot_timer, portMAX_DELAY);
    }
  }
}

/*
  Setup
 */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Starting Timer Demo"); 

  // take sometime for user to push serial console button
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  one_shot_timer = xTimerCreate(
    "one-shot timer",         // name
    dim_delay_period,         // period of timer
    pdFALSE,                  // auto reload
    (void *)0,                // id
    autoDimmerCallback        // callback
  );

  xTaskCreatePinnedToCore(
    readSerial,               // name
    "Read serial",
    1024, 
    NULL, 
    1, 
    NULL,
    app_cpu
  );
}

void loop() {
  // put your main code here, to run repeatedly:

}
