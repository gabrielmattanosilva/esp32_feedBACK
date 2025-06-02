#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <MadgwickAHRS.h>
#include "Pins.h"

MPU6050 mpu;
Madgwick filter;

// Definições
#define SAMPLE_RATE 100 // Hz

// Variáveis para armazenar os dados do sensor
int16_t ax, ay, az;
int16_t gx, gy, gz;
float roll, pitch, yaw;

unsigned long microsPerReading, microsPrevious;
unsigned long microsNow;

void setup() {
  Serial.begin(115200);
  while (!Serial); // Aguarda a conexão serial no ESP32

  Wire.begin(PIN_SDA, PIN_SCL);
  
  // Inicializa o MPU6050
  Serial.println("Inicializando MPU6050...");
  mpu.initialize();
  
  // Verifica a conexão
  if (!mpu.testConnection()) {
    Serial.println("Falha ao conectar ao MPU6050!");
    while (1) {
      delay(10);
    }
  }
  
  // Configura o filtro Madgwick
  filter.begin(SAMPLE_RATE);
  
  // Calcula o tempo entre leituras
  microsPerReading = 1000000 / SAMPLE_RATE;
  microsPrevious = micros();
}

void loop() {
  // Verifica se é hora de ler os dados
  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {
    // Lê os dados do sensor
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    // Converte os valores para unidades adequadas
    // Acelerômetro: converte para g's (considerando range de ±2g)
    float accX = ax / 16384.0;
    float accY = ay / 16384.0;
    float accZ = az / 16384.0;
    
    // Giroscópio: converte para rad/s (considerando range de ±250°/s)
    float gyroX = gx / 131.0 * (PI / 180.0);
    float gyroY = gy / 131.0 * (PI / 180.0);
    float gyroZ = gz / 131.0 * (PI / 180.0);
    
    // Atualiza o filtro com os novos dados
    filter.updateIMU(gyroX, gyroY, gyroZ, accX, accY, accZ);
    
    // Obtém os ângulos de Euler
    roll = filter.getRoll();
    pitch = filter.getPitch();
    yaw = filter.getYaw();
    
    // Exibe os resultados no monitor serial
    Serial.print("Roll: ");
    Serial.print(roll);
    Serial.print(" Pitch: ");
    Serial.print(pitch);
    Serial.print(" Yaw: ");
    Serial.println(yaw);
    
    // Atualiza o tempo da última leitura
    microsPrevious = microsPrevious + microsPerReading;
  }
}