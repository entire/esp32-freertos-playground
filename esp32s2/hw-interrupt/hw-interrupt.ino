
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const int16_t timer_divider = 80; // clock divider, clock ticks at 1mhz
static const int64_t timer_max_count = 1000000; // 1 Mhz
static const TickType_t task_delay = 2000 / portTICK_PERIOD_MS; // 2 secs

// Settings
static const uint8_t buf_len = 20;

// Pins
static const int led_pin = LED_BUILTIN;

// Globals
static hw_timer_t *timer = NULL;
static volatile int isr_counter;
static portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED; // spinlock is ESP32 specific 

typedef struct {
    uint8_t * const buffer;
    int head;
    int tail;
    const int maxlen;
} adc_sample_circ_bbuf_t;

int circ_bbuf_push(adc_sample_circ_bbuf_t *c, uint8_t data)
{
    int next;

    next = c->head + 1;  // next is where head will point to after this write.
    if (next >= c->maxlen)
        next = 0;

    if (next == c->tail)  // if the head + 1 == tail, circular buffer is full
        return -1;

    c->buffer[c->head] = data;  // Load data and then move
    c->head = next;             // head to next data offset.
    return 0;  // return success to indicate successful push.
}

int circ_bbuf_pop(circ_bbuf_t *c, uint8_t *data)
{
    int next;

    if (c->head == c->tail)  // if the head == tail, we don't have any data
        return -1;

    next = c->tail + 1;  // next is where tail will point to after this read.
    if(next >= c->maxlen)
        next = 0;

    *data = c->buffer[c->tail];  // Read data and then move
    c->tail = next;              // tail to next offset.
    return 0;  // return success to indicate successful push.
}

//*****************************************************************************
// ISR

// this function executes when timer reaches max (and resets)
void IRAM_ATTR onTimer() {

  // Toggle LED
  int pin_state = digitalRead(led_pin);
  digitalWrite(led_pin, !pin_state);
}

//*****************************************************************************
// Callbacks

void readSerial(void *parameters) {

  // char c;
  // char buf[buf_len];

  
  // configure pins
  // pinMode(led_pin, OUTPUT);

  // memset(buf, 0, buf_len);

  // loop forever
  // while (1) {
  //   if (Serial.available() > 0) {
  //     c = Serial.read();

  //   if (c == '\n') {
      
  //   } else {

  //   }
  // }
}


//*****************************************************************************
// Main

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Starting HW Interrupt Demo"); 

  // create and start timer (num, divider, countUp)
  timer = timerBegin(0, timer_divider, true);

  // provide ISR to timer (timer, function, edge)
  timerAttachInterrupt(timer, &onTimer, true);

  // at what count should ISR trigger (timer, count, autoreload)
  timerAlarmWrite(timer, timer_max_count, true);

  // allow ISR to trigger
  timerAlarmEnable(timer);


  xTaskCreatePinnedToCore(
    readSerial,     // Function to be called
    "Read Serial",  // Name of task
    1024,           // Stack size (bytes in ESP32, words in FreeRTOS)
    NULL,           // Parameter to pass
    1,              // Task priority (must be same to prevent lockup)
    NULL,           // Task handle
    app_cpu         // Run on one core for demo purposes (ESP32 only)
  );

}

void loop() {
  // do nothing
}