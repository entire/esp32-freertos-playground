
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const int16_t timer_divider = 8; // clock ticks at 10mhz
static const int64_t timer_max_count = 1000000;
static const TickType_t task_delay = 2000 / portTICK_PERIOD_MS; // 2 secs

// Settings
static const uint8_t buf_len = 20;

// Pins
static const int led_pin = LED_BUILTIN;

// Globals
static hw_timer_t *timer = NULL;

//*****************************************************************************
// ISR

// this function executes when timer reaches max (and resets)
void IRAM_ATTR onTimer() {
  
}

//*****************************************************************************
// Callbacks

void readSerial(void *parameters) {

  char c;
  char buf[buf_len];

  
  // configure pins
  pinMode(led_pin, OUTPUT);

  memset(buf, 0, buf_len);

  // loop forever
  while (1) {
    if (Serial.available() > 0) {
      c = Serial.read();

    if (c == '\n') {
      
    } else {

    }
  }
}


//*****************************************************************************
// Main

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Starting HW Interrupt Demo"); 

  xTaskCreatePinnedToCore(
    readSerial,     // Function to be called
    "Read Serial",  // Name of task
    1024,           // Stack size (bytes in ESP32, words in FreeRTOS)
    NULL,           // Parameter to pass
    1,              // Task priority (must be same to prevent lockup)
    NULL,           // Task handle
    app_cpu         // Run on one core for demo purposes (ESP32 only)
  );

  // delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {

}