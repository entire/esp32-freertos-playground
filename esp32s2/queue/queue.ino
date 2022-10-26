#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// settings
static const uint8_t msg_queue_len = 5;

// globals
static QueueHandle_t msg_queue;

// tasks

// task: wait for item on queue and print it
void printMessages(void *parameters) {
  int item;

  // forever loop
  while (1) {
  
    // wait to see if there's a message
    if (xQueueReceive(msg_queue, (void *)&item, 0) == pdTRUE) {
      Serial.println(item);  
    }

    // wait
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  
  // wait till serial is ready
  while(!Serial);
  Serial.println("Welcome to Queue demo");
  
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  xTaskCreatePinnedToCore( // use xTaskCreate() in valline FreeRTOS
    printMessages,    // function to be called
    "Print Messages", // name of task
    1024,             // stack size
    NULL,             // parameter to pass to function
    1,                // task priority, 1 is max (0 to configMAX_PRIORITIES - 1)
    NULL,             // task handle
    app_cpu);         // run on one core for demo purposes (ESP32 only) 
}

void loop() {
  // put your main code here, to run repeatedly:

  static int num = 0;

  if (xQueueSend(msg_queue, (void *)&num, 10) != pdTRUE) {
    Serial.println("Queue Full");
  }
  num++;

  // delay 1 sec before trying again
  vTaskDelay(500 / portTICK_PERIOD_MS);
}
