
# Proyecto Final - Monitoreo Ambiental con ESP32

Este proyecto utiliza un microcontrolador **ESP32** para monitorear diversos parámetros ambientales mediante sensores y enviar los datos recopilados a la plataforma **ThingSpeak**. Además, los datos se muestran en una pantalla LCD conectada al dispositivo.

## Funcionalidades

1. **Monitoreo de calidad del aire** utilizando el sensor MQ-135.
2. **Medición de temperatura y humedad** con el sensor DHT11.
3. **Detección de radiación UV** mediante un sensor UV.
4. **Visualización de datos** en una pantalla LCD (LiquidCrystal_I2C).
5. **Transmisión de datos a la nube** a través de WiFi utilizando **ThingSpeak**.

## Componentes Utilizados

- **ESP32**: Microcontrolador principal.
- **MQ-135**: Sensor de calidad del aire.
- **DHT11**: Sensor de temperatura y humedad.
- **Sensor UV**: Para medir niveles de radiación ultravioleta.
- **Pantalla LCD I2C**: Para visualizar los datos en tiempo real.
- **WiFi**: Para transmitir los datos a ThingSpeak.

## Librerías Requeridas

Antes de compilar y cargar el código, asegúrate de instalar las siguientes librerías en tu entorno de desarrollo Arduino:

- [MQUnifiedsensor](https://github.com/miguel5612/MQSensorsLib)
- [WiFi](https://www.arduino.cc/en/Reference/WiFi)
- [ThingSpeak](https://github.com/mathworks/thingspeak-arduino)
- [DHT](https://github.com/adafruit/DHT-sensor-library)
- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)

## Configuración

1. **Configurar ThingSpeak**: 
   - Crea una cuenta en [ThingSpeak](https://thingspeak.com/) y configura un canal para almacenar los datos.
   - Obtén tu API Key y reemplázala en el código donde corresponda.

2. **Conexión de los Sensores**:
   - MQ-135: Conecta al pin 32 del ESP32.
   - DHT11: Conecta al pin 23 del ESP32.
   - Sensor UV: Conecta al pin 33 del ESP32.
   - LCD I2C: Conéctala a los pines SDA y SCL del ESP32.

3. **Configuración de WiFi**:
   - Ingresa el nombre de tu red WiFi y la contraseña en las variables correspondientes del código.

## Uso

1. Carga el código en el ESP32 utilizando el IDE de Arduino.
2. Conecta el ESP32 a una fuente de alimentación.
3. Observa los datos en tiempo real en la pantalla LCD.
4. Visualiza los datos históricos y gráficos en el canal configurado en ThingSpeak.

## Créditos

Desarrollado por Jhon Machado.
