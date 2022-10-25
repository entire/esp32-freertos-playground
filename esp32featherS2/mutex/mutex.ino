#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// globals
static int shared_variable = 0;
static SemaphoreHandle_t mutex;

/* 
  tasks 
*/
void incrementTask(void *parameters) {
  int local_variable;

  while(1) {
    // take mutex prior to critical section
    if (xSemaphoreTake(mutex, 0) == pdTRUE) {

      local_variable = shared_variable;
      local_variable++;
      vTaskDelay(random(100, 500) / portTICK_PERIOD_MS);
      shared_variable = local_variable;

      // print it
      Serial.println(shared_variable);

      // give mutex back after critical section
      xSemaphoreGive(mutex);

    } else {
      Serial.println("could not get mutex");
    }

  }
}

void setup() {
  // Hack to kinda get randomness
  randomSeed(analogRead(0));

  // put your setup code here, to run once:
  Serial.begin(115200);

  // wait till serial is ready
  while(!Serial);
  Serial.println("Welcome to FreeRTOS Mutex demo");

  // create mutex first
  mutex = xSemaphoreCreateMutex();
  
  xTaskCreatePinnedToCore( // use xTaskCreate() in line FreeRTOS
    incrementTask,    // function to be called
    "inc task1",      // name of task
    1024,             // stack size
    NULL,             // parameter to pass to function
    1,                // task priority, 1 is max (0 to configMAX_PRIORITIES - 1)
    NULL,             // task handle
    app_cpu);         // run on one core for demo purposes (ESP32 only) 

  xTaskCreatePinnedToCore( // use xTaskCreate() in line FreeRTOS
    incrementTask,    // function to be called
    "inc task2",      // name of task
    1024,             // stack size
    NULL,             // parameter to pass to function
    1,                // task priority, 1 is max (0 to configMAX_PRIORITIES - 1)
    NULL,             // task handle
    app_cpu);         // run on one core for demo purposes (ESP32 only) 
  
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}
