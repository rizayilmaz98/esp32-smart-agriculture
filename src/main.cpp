#include <Arduino.h>
#include <string>

#include <MySQL_Generic_WiFi.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include "Adafruit_Si7021.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <ESP32Servo.h>

#include "defines.h"
#include "Credentials.h"
#include "DefaultDevice.h"
#include "AktarsisImage.h"
#include "SetQuery.h"

#define WIFI_TIMEOUT_MS 20000

#define DS1302_CLK_PIN 27
#define DS1302_DATA_PIN 26
#define DS1302_RST_PIN 25

#define LIGTHNING_PIN 18
#define SERVO_PIN 12
#define SOLAR_PANEL_PIN 32
#define SOIL_HUMIDITY_PIN 34
#define PH_PIN 35

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//Task Handles
TaskHandle_t wificontask_handle = NULL;
TaskHandle_t moisturtask_handle = NULL;
TaskHandle_t temptask_handle = NULL;
TaskHandle_t rtctask_handle = NULL;

//Server Connection Definitions
IPAddress server_addr(35, 246, 141, 43); //Google-cloud-platform public ip address
uint16_t server_port = 3306;    //3306;

//Declaration for MySQL Server Connection
MySQL_Connection conn((Client *)&client);

//Declaration for Real Time Counter
ThreeWire myWire(DS1302_DATA_PIN, DS1302_CLK_PIN, DS1302_RST_PIN); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

//Declaration for Si7021 Temperature and Relative Humidity Sensor
Adafruit_Si7021 sensor = Adafruit_Si7021();

//Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Declaration for Feeding Control
Servo aktarsiservo;

//Declaration for Query Transform
char transform_query[150];

//Declarations for Device info
String user_id;
String plant_id;
String soil_id;
String location_info;
String device_ph;
String device_time;

//Declarations for Device Parameters
String ideal_temp_min;
String ideal_temp_max;
String nonideal_temp_min;
String nonideal_temp_max;
String hum_min;
String hum_max;
String ph_min;
String ph_max;
String feeding_range;

//Declarations for sensors parameters
uint16_t moisture_bit;
uint16_t pH_bit;
double moisture;
double pH_val;
double air_hum;
double air_temp;

char temp_sensor[10];
char hum_sensor[10];
char moisture_sensor[10];
char ph_sensor[10];

//Definition for lighning control
String lightning_time = String("23:00:00");
String lightning_stop = String("24:00:00");

//Declarations for date-time
char datestring[20];
char timestring[9];

//Declarations for OLED Display Text
String wiFiText;
String serverText;

//Declarations for Device Control Parameters
double it_min;
double it_max;
double nt_min;
double nt_max;
double h_min;
double h_max;
double p_min;
double p_max;

//Definition for Servo position
unsigned short int pos = 180; //close - 0 to open

//Definitions and Declarations for Power Calculating
uint32_t volt_bit;
double volt;
double amper;
double generated_power;
double consumed_power;
const double peak_power = 10.00;
const double peak_current = 0.55;
const double peak_volt = 18.18;
const double power_adaptor = 18.00;

char avrg_gen_power[10];
char avrg_con_power[10];

//Declaration for feeding counter
unsigned int feeding_counter;
int feed_range;

void setWifiConnection(){
  Serial.println(String("Connecting to ") + ssid);
  WiFi.begin(ssid, pass);

  unsigned long startAttemptTimeWiFi = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTimeWiFi < WIFI_TIMEOUT_MS)
  {
    /* code */
    Serial.print(".");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }

  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Failed!");
  }
  else{
    Serial.print("Connected to wifi!");
    Serial.print(WiFi.localIP());
  }
  
}

void setDeviceParameters(void)
{
  String query = SetQuery::setDeviceParametersQuery() + plant_id;

  Serial.println("====================================================");
  Serial.println("> Running SELECT with dynamically supplied parameter");
  
  Serial.println(query);
  
  // Initiate the query class instance
  MySQL_Query query_mem = MySQL_Query(&conn);

  if (!query_mem.execute(query.c_str()))
  {
    Serial.println("Querying error");
    return;
  }
  
  // Fetch the columns and print them
  column_names *cols = query_mem.get_columns();

  // Read the rows and print them
  row_values *row = NULL;
  
  do 
  {
    row = query_mem.get_next_row();
    
    if (row != NULL) 
    {
      for (int f = 0; f < cols->num_fields; f++) 
      {

        if(f == 4)
        {
          ideal_temp_min = row->values[f];
        }
        else if(f == 5)
        {
          ideal_temp_max = row->values[f];
        }
        else if(f == 6)
        {
          nonideal_temp_min = row->values[f];
        }
        else if(f == 7)
        {
          nonideal_temp_max = row->values[f];
        }
        else if(f == 8)
        {
          hum_min = row->values[f];
        }
        else if (f == 9)
        {
          hum_max = row->values[f];
        }
        else if(f == 10)
        {
          ph_min = row->values[f];
        }
        else if (f == 11)
        {
          ph_max = row->values[f];
        }
        else if(f == 13)
        {
          feeding_range = row->values[f];
        }
      }
    }
  } while (row != NULL);
}


void setDeviceInfo(void)
{
  String query = SetQuery::setDeviceInfoQuery();

  Serial.println("====================================================");
  Serial.println("> Running SELECT with dynamically supplied parameter");
  
  Serial.println(query);
  
  // Initiate the query class instance
  MySQL_Query query_mem = MySQL_Query(&conn);

  if (!query_mem.execute(query.c_str()))
  {
    Serial.println("Querying error");
    return;
  }
  
  // Fetch the columns and print them
  column_names *cols = query_mem.get_columns();

  // Read the rows and print them
  row_values *row = NULL;
  
  do 
  {
    row = query_mem.get_next_row();
    
    if (row != NULL) 
    {
      for (int f = 0; f < cols->num_fields; f++) 
      {

        if(f == 1)
        {
          user_id = row->values[f];
        }
        else if(f == 2)
        {
          plant_id = row->values[f];
        }
        else if(f == 3)
        {
          soil_id = row->values[f];
        }
        else if(f == 4)
        {
          location_info = row->values[f];
        }
        else if(f == 5)
        {
          device_ph = row->values[f];
        }
        else if (f == 6)
        {
          device_time = row->values[f];
        }
      }
    }
  } while (row != NULL);
}

void insertDeviceProccessTable(char* proccess_id)
{
  // Initiate the query class instance
  MySQL_Query query_mem = MySQL_Query(&conn);

  String query = SetQuery::setInsertDeviceProccessQuery();

  if (conn.connected())
  {
    //Double to String
    dtostrf(air_temp, 1, 1, temp_sensor);
    dtostrf(air_hum, 1, 1, hum_sensor);
    dtostrf(moisture, 1, 1, moisture_sensor);
    dtostrf(pH_val, 1, 1, ph_sensor);

    //Add valuees into query
    sprintf(transform_query, 
            query.c_str(),
            device_id,
            proccess_id,
            datestring,
            timestring,
            temp_sensor,
            hum_sensor,
            moisture_sensor,
            ph_sensor);
    
    // Execute the query
    Serial.println(transform_query);

    // KH, check if valid before fetching
    if (!query_mem.execute(transform_query))
      Serial.println("Complex Insert error");
    else    
      Serial.println("Complex Data Inserted.");
  }
  else
  {
    Serial.println("Disconnected from Server. Can't insert.");
  }
}

void insertPanelTable(void)
{
  // Initiate the query class instance
  MySQL_Query query_mem = MySQL_Query(&conn);

  String query = SetQuery::setPanelProccessQuery();

  if (conn.connected())
  {
    //Double to String
    dtostrf(peak_power, 1, 1, avrg_gen_power);
    dtostrf(power_adaptor, 1, 1, avrg_con_power);

    //Add valuees into query
    sprintf(transform_query, 
            query.c_str(),
            device_id,
            datestring,
            avrg_gen_power,
            avrg_con_power);
    
    // Execute the query
    Serial.println(transform_query);

    // KH, check if valid before fetching
    if (!query_mem.execute(transform_query))
      Serial.println("Complex Insert error");
    else    
      Serial.println("Complex Data Inserted.");
  }
  else
  {
    Serial.println("Disconnected from Server. Can't insert.");
  }
}

void setServerConnection()
{
  Serial.print("Connecting to SQL Server @ ");
  Serial.print(server_addr);
  Serial.println(String(", Port = ") + server_port);
  Serial.println(String("User = ") + user + String(", PW = ") + password + String(", DB = ") + database_name);
  if (conn.connectNonBlocking(server_addr, server_port, user, password) != RESULT_FAIL)
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    //Set Device Info
    setDeviceInfo();

    vTaskDelay(1000 / portTICK_PERIOD_MS);
    //Set Device Parameters
    setDeviceParameters();

    serverText = String("Server Connected to:");
  } 
  else 
  {
    serverText = String("Server Connection Failed to:");
    Serial.println("\nConnect failed. Trying again on next iteration.");

    user_id = default_user_id;
    plant_id = default_plant_id;
    soil_id = default_soil_id;
    location_info = default_location;
    device_ph = default_ph;
    device_time = default_feeding_time;

    ideal_temp_min = default_ideal_temp_min;
    ideal_temp_max = default_ideal_temp_max;
    nonideal_temp_min = default_nonideal_temp_min;
    nonideal_temp_max = default_nonideal_temp_max;
    hum_min = default_hum_min;
    hum_max = default_hum_max;
    ph_min = default_ph_min;
    ph_max = default_ph_max;
  }

}

void phControl()
{
  /*PH Control Code*/
  pH_val = atof(device_ph.c_str());

  Serial.println(pH_val);
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void checkDateTime(const RtcDateTime& dt)
{
  sscanf(feeding_range.c_str(), "%d", &feed_range);

  snprintf_P(datestring, 
          countof(datestring),
          PSTR("%04u-%02u-%02u"),
          dt.Year(),
          dt.Month(),
          dt.Day());
  
  snprintf_P(timestring, 
          countof(timestring),
          PSTR("%02u:%02u:%02u"),
          dt.Hour(),
          dt.Minute(),
          dt.Second());

  if(String(timestring) == device_time && (feeding_counter % feed_range) == 0)
  {
    pos = 0;
    aktarsiservo.write(pos);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    insertDeviceProccessTable(&feeding_id);
    insertDeviceProccessTable(&measurment_id);
  }
  else if(String(timestring) == lightning_time)
  {
    digitalWrite(LIGTHNING_PIN, LOW);
    insertDeviceProccessTable(&lightning_id);
    insertPanelTable();
  }
  else if(String(timestring) == lightning_stop)
  {
    digitalWrite(LIGTHNING_PIN, HIGH);
    feeding_counter ++;
  }
}

void WiFiConnectionTask(void* parameter)
{
  for(;;)
  {

    if(WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Failed! Trying to set WiFi Connection..");
      wiFiText = String("WiFi Connection Failed!");
      setWifiConnection();

      user_id = default_user_id;
      plant_id = default_plant_id;
      soil_id = default_soil_id;
      location_info = default_location;
      device_ph = default_ph;
      device_time = default_feeding_time;

      ideal_temp_min = default_ideal_temp_min;
      ideal_temp_max = default_ideal_temp_max;
      nonideal_temp_min = default_nonideal_temp_min;
      nonideal_temp_max = default_nonideal_temp_max;
      hum_min = default_hum_min;
      hum_max = default_hum_max;
      ph_min = default_ph_min;
      ph_max = default_ph_max;

    }
    else{
      Serial.print("Connected To Wifi!");
      Serial.print(WiFi.localIP());

      if(conn.connected())
      {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        //Set Device Info
        setDeviceInfo();

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        //Set Device Parameters
        setDeviceParameters();
      }
      else
      {
        setServerConnection();
      }

      wiFiText = String("WiFi Connected to:");     
    }

    vTaskDelay(60000 / portTICK_PERIOD_MS);
  }
}

void rtcTask(void* parameter)
{
  for(;;)
  {
    RtcDateTime now = Rtc.GetDateTime();
    checkDateTime(now);

    if(!now.IsValid())
    {
      Serial.println("RTC lost confidence in the DateTime!");
    }

    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void soilHumidityTask(void* parameter)
{

  for(;;)
  {
    h_min = atof(hum_min.c_str());
    h_max = atof(hum_max.c_str());

    moisture_bit = 4096 - analogRead(SOIL_HUMIDITY_PIN);
    moisture = map(moisture_bit, 0, 4095, 0, 100);

    Serial.println(moisture);
    phControl();

    if(moisture >= h_min && moisture <= h_max)
    {
      /*Ideal humidity range code*/
      Serial.println("Ideal humidity range");
    }
    else if(moisture < h_min)
    {
      /*Ideal humidity out of range code*/
      Serial.println("Humidity is below the ideal min moisture");

      //feedingControl();
      if(pos == 180)
      {
        pos = 0;
        aktarsiservo.write(pos);
        insertDeviceProccessTable(&feeding_id);
      }
    }
    else if(moisture > h_max)
    {
      /*Ideal Humidity out of range code*/
      Serial.println("Humidty is above the ideal max mositure");
      if(pos == 0)
      {
        pos = 180;
        aktarsiservo.write(pos);
      }
    }

    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void temperatureTask(void* parameter)
{

  for(;;)
  {
    /*Si7021 Temperature and Humidty Sensor Code*/
    it_min = atof(ideal_temp_min.c_str());
    it_max = atof(ideal_temp_max.c_str());
    nt_min = atof(nonideal_temp_min.c_str());
    nt_max = atof(nonideal_temp_max.c_str());

    air_hum = sensor.readHumidity();
    air_temp = sensor.readTemperature();
    
    Serial.print("Humidity:    ");
    Serial.print(air_hum, 2);
    Serial.print("\tTemperature: ");
    Serial.println(air_temp, 2);

    if(air_temp >= it_min && air_temp <= it_max)
    {
      Serial.println("Temperature is between the ideal temps rate");
    }
    else if(air_temp < nt_min)
    {
      Serial.println("WARNING!!! Temperature is below the nonideal min temp");
    }
    else if(air_temp > nt_max)
    {
      Serial.println("WARNING!!! Temperature is above the nonideal max temp");
    }
    else 
    {
      Serial.println("Temperature is between the ideal and nonideal temps rate");
    }

    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void aktarsisLogoDisplay()
{
  display.clearDisplay();
  display.drawBitmap(0, 0, AktarsisImage::image_data_aktarsis_image, 128, 64, 1);
  display.display();
  display.startscrollright(0x00, 0x0F);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  display.stopscroll();
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  display.startscrollleft(0x00, 0x0F);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  display.stopscroll();
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  display.startscrolldiagright(0x00, 0x07);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  display.startscrolldiagleft(0x00, 0x07);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  display.stopscroll();
}

void wiFiServerConDisplay()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  // clear display
  display.clearDisplay();
  
  // display WiFi Connection
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print(wiFiText);
  display.setTextSize(1);
  display.setCursor(0,10);
  display.print(WiFi.localIP());
  display.cp437(true);
  display.write(167);

  
  // display Server Connection
  display.setTextSize(1);
  display.setCursor(0, 25);
  display.print(serverText);
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print(hostname);
  
  display.display(); 
}

void tempHumDisplay()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  // clear display
  display.clearDisplay();
  
  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(air_temp);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(air_hum);
  display.print(" %"); 
  
  display.display(); 
}

void moisturePHDisplay()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  // clear display
  display.clearDisplay();
  
  // display PH
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("pH Value: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(device_ph);
  display.cp437(true);
  display.write(167);
  
  // display Moisture
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Moisture: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(moisture);
  display.print(" %"); 
  
  display.display(); 
}

void powerDisplay()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.clearDisplay();
  
  // display generated power
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Generated Power:");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(peak_power);
  display.print(" W"); 
  display.cp437(true);
  display.write(167);
  
  // display consumed power
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Consumed Power: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(power_adaptor);
  display.print(" W"); 
  
  display.display(); 
}

void dateTimeDisplay()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.clearDisplay();
  
  // display date
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Date:");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(datestring);
  display.cp437(true);
  display.write(167);
  
  // display time
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Time: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(timestring);
  
  display.display(); 
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  //Moisture
  pinMode(SOIL_HUMIDITY_PIN, INPUT);

  //ligthing pin
  pinMode(LIGTHNING_PIN, OUTPUT);
  digitalWrite(LIGTHNING_PIN, HIGH);
  
  //DS1306
  Rtc.Begin();
  //RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  //Rtc.SetDateTime(compiled);

  //Si7021
  sensor.begin();

  //MG90S Servo
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	aktarsiservo.setPeriodHertz(50);    // standard 50 hz servo
	aktarsiservo.attach(SERVO_PIN, 1000, 2000);

  Serial.println("\nStarting Connect_WiFi on " + String(BOARD_NAME));

  // Remember to initialize your WiFi module
#if ( USING_WIFI_ESP8266_AT  || USING_WIFIESPAT_LIB ) 
  #if ( USING_WIFI_ESP8266_AT )
    Serial.println("Using ESP8266_AT/ESP8266_AT_WebServer Library");
  #elif ( USING_WIFIESPAT_LIB )
    Serial.println("Using WiFiEspAT Library");
  #endif
  
  // initialize serial for ESP module
  EspSerial.begin(115200);
  // initialize ESP module
  WiFi.init(&EspSerial);

  Serial.println(F("WiFi shield init done"));

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println(F("WiFi shield not present"));
    // don't continue
    while (true);
  }
#endif

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  //WiFi Task
  xTaskCreate(
    WiFiConnectionTask,
    "Set WiFi Connection and Keep WiFi Connection Alive Task",
    4096,
    NULL,
    1,
    &wificontask_handle
  );

  //RTC Task
  xTaskCreate(
    rtcTask,
    "Real Time Counter Task",
    8192,
    NULL,
    1,
    &rtctask_handle
  );

  //Mositure Measurment Task
  xTaskCreate(
    soilHumidityTask,
    "Soil Humidity - Moisture Measurment and Control Task",
    4096,
    NULL,
    1,
    &moisturtask_handle
  );

  //Temperature Measurment Task
  xTaskCreate(
    temperatureTask,
    "Si7021 Air Temperature and Humidty Measurment Task",
    4096,
    NULL,
    1,
    &temptask_handle
  );
}

void loop()
{
  aktarsisLogoDisplay();
  vTaskDelay(2000 / portTICK_PERIOD_MS); 
  wiFiServerConDisplay();
  vTaskDelay(3000 / portTICK_PERIOD_MS); 
  tempHumDisplay();
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  moisturePHDisplay();
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  powerDisplay();
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  dateTimeDisplay();
  vTaskDelay(3000 / portTICK_PERIOD_MS);
}