#include "esp_wifi.h"
#include "esp_bt_main.h"
#include "esp_bt.h"
#include "Wire.h"
#include "LSM303.h"


RTC_DATA_ATTR int bootCount = 0;
LSM303 acc_mag_sensor;

void setup(){
	Serial.begin(115200);

	Wire.setClock(400000); // i2C is needed to get data from magnetometer.

	delay(1000); //Take some time to open up the Serial Monitor

	//Increment boot number and print it every reboot
	++bootCount;

	if (bootCount == 0)
	{
		//configure the LSM303 only once after the initial boot
	}
	else
	{
		// After the initial boot, ESP32 is woken up only through an external interrupt
		/* code for reading data from magnetometer */
	}


	Serial.println("Boot number: " + String(bootCount));

	touch_pad_intr_disable();

	//Print the wakeup reason for ESP32
	PrintWakeupReason();


}

void loop(){}

//Function that prints the reason by which ESP32 has been awaken from sleep
void PrintWakeupReason(){
	esp_sleep_wakeup_cause_t wakeup_reason;
	wakeup_reason = esp_sleep_get_wakeup_cause();
	switch(wakeup_reason)
	{
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
	}
}

void GoToSleep()
{
	esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,HIGH);
	esp_bluedroid_disable();
	esp_bt_controller_disable();
	esp_wifi_stop();
	//Go to sleep now
	esp_deep_sleep_start();
}
