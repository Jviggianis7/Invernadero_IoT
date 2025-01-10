#include <MQUnifiedsensor.h>
#include <WiFi.h>
#include <Wire.h>
#include "ThingSpeak.h"
#include "DHT.h"
#include <LiquidCrystal_I2C.h>


// Definiciones para el sensor MQ-135
#define placa "ESP32"
#define Voltage_Resolution 3.3
#define pin 32
#define type "MQ-135"
#define ADC_Bit_Resolution 12
#define RatioMQ135CleanAir 3.6

// Definiciones para el sensor UV
int const UV_SENSOR_PIN = 33;

// Definiciones para el sensor DHT11
#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Definiciones para el lcd
int GPIO_SDA = 21;
int GPIO_SCL = 22;

LiquidCrystal_I2C lcd_i2c(0x27, 16, 2);


// Definiciones para WiFi y ThingSpeak
const char* ssid = "Admin";
const char* password = "AdminNet2023@";
WiFiClient client;
unsigned long myChannelNumber = 2586159;
const char* myWriteAPIKey = "DTT5M9M5OIQP7YRG";

// Declarar el sensor MQ-135
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup() {
  Serial.begin(115200);

  // Inicializar sensor MQ-135
  MQ135.setRegressionMethod(1);
  MQ135.init();

  Serial.print("Calibrando, por favor espera.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ135.update();
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0 / 10);
  Serial.println("  ¡hecho!");

  if (isinf(calcR0)) {
    Serial.println("Advertencia: Problema de conexión encontrado, R0 es infinito (circuito abierto detectado), por favor revisar el cableado y suministro");
    while (1);
  }
  if (calcR0 == 0) {
    Serial.println("Advertencia: Problema de conexión encontrado, R0 es cero (pin analógico con cortocircuito a tierra), por favor revisa el cableado y suministro");
    while (1)
      ;
  }
  
  Wire.begin(GPIO_SDA, GPIO_SCL);

  Serial.println("PRUEBA SENSORES");

  // Conectar a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Conectado");
  ThingSpeak.begin(client);

  // Inicializar sensor DHT
  dht.begin();

  //Mensaje inicial del lcd
  Serial.println("Test de sensores");
  lcd_i2c.begin(16, 2);
  lcd_i2c.backlight();
  lcd_i2c.setCursor(0, 0);
  lcd_i2c.println("Test de sensores");
  lcd_i2c.setCursor(0, 1);
  lcd_i2c.print("AQI,Temp,Hum,UV");
  delay(2000);
}

void loop() {

  LeerMQ135();
  delay(2000);
  LeerDHT11();
  delay(2000);
  LeerUV();
  delay(2000);
  
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  Serial.println("Datos enviados correctamente!");
  lcd_i2c.clear();
  lcd_i2c.setCursor(0, 0);
  lcd_i2c.print("Datos enviados!");

  delay(10000);
}

void LeerMQ135() {
  // Leer sensor MQ-135
  MQ135.update();
  MQ135.setA(110.47);
  MQ135.setB(-2.862);
  float CO2 = MQ135.readSensor();
  Serial.print("CO2: ");
  Serial.print(CO2);
  Serial.println(" ppm");

  lcd_i2c.clear();
  lcd_i2c.setCursor(0, 0);
  lcd_i2c.print("AQ: ");
  lcd_i2c.print(CO2);



  ThingSpeak.setField(1, CO2);
}

void LeerDHT11() {
  // Leer sensor DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  lcd_i2c.clear();

  if (isnan(h) || isnan(t)) {
    lcd_i2c.setCursor(0, 0);
    lcd_i2c.print("Temp: Error");
    lcd_i2c.setCursor(0, 1);
    lcd_i2c.print("Hum: Error");
    Serial.println("Error al leer el sensor DHT");
  } else {

    lcd_i2c.setCursor(0, 0);
    lcd_i2c.print("Temp: ");
    lcd_i2c.print(t);
    lcd_i2c.print(" °C");

    lcd_i2c.setCursor(0, 1);
    lcd_i2c.print("Hum: ");
    lcd_i2c.print(h);
    lcd_i2c.print(" %");

    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.print(" °C, Humedad: ");
    Serial.print(h);
    Serial.println(" %");

    ThingSpeak.setField(2, t);
    ThingSpeak.setField(3, h);
  }
}
void LeerUV() {
  // Leer sensor UV
  int sensor_value = analogRead(UV_SENSOR_PIN);
  float volts = sensor_value * 5.0 / 4095.0;
  float UV_index = volts * 10;
  Serial.print("UV Index: ");
  Serial.println(UV_index);

  lcd_i2c.clear();
  lcd_i2c.setCursor(0, 0);
  lcd_i2c.print("UV: ");
  lcd_i2c.print(UV_index);
  lcd_i2c.print("     ");

  if (UV_index < 3) {
    lcd_i2c.setCursor(0, 1);
    lcd_i2c.print("UV bajo");
    Serial.print("UV bajo");
  } else if (UV_index >= 3 && UV_index < 6) {
    lcd_i2c.setCursor(0, 1);
    lcd_i2c.print("UV moderado");
    Serial.print("UV moderado");
  } else if (UV_index >= 6 && UV_index < 8) {
    lcd_i2c.setCursor(0, 1);
    lcd_i2c.print("UV alto");
    Serial.print("UV alto");
  } else if (UV_index >= 8 && UV_index < 11) {
    lcd_i2c.setCursor(0, 1);
    lcd_i2c.print("UV my alto");
    Serial.print("UV muy alto");
  } else {
    lcd_i2c.setCursor(0, 1);
    lcd_i2c.print("UV peligroso");
    Serial.print("UV extremadamente alto");
  }

  ThingSpeak.setField(4, UV_index);
}
