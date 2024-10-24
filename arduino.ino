#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Inicializa o MPU6050
  if (!mpu.begin()) {
    Serial.println("Não foi possível encontrar um MPU6050!");
    while (1); // Fica preso se não conectar
  }

  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calcular os ângulos de pitch e roll
  float pitch = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI;
  float roll = atan2(a.acceleration.x, a.acceleration.z) * 180.0 / PI;

  // Enviar os ângulos via Serial
  Serial.print(pitch);
  Serial.print(",");
  Serial.println(roll);
  
  delay(100);
}
