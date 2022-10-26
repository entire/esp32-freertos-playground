#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;

void myTimerCallback(TimerHandle_t xTimer) {
  if ((uint32_t)pvTimerGetTimer)
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // wait till serial is ready
  while(!Serial);
  Serial.println("Welcome to FreeRTOS timer demo");

  one_shot_timer = xTimerCreate(
    "One shot timer",           // name of timer
    2000 / portTICK_PERIOD_MS,  // period of timer in ticks
    pdFALSE,
    (void *)0,                  // timer id
    myTimerCallback
  );

  if (one_shot_timer == NULL) {
    // could not create 
    Serial.println("could not create one shot timer");
  }

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Starting Timers...");

  xTimerStart(one_shot_timer, portMAX_DELAY);

  vTaskDelete(NULL);
}

void loop() {

}