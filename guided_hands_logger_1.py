import serial
from datetime import datetime
import time

# Configurações
SERIAL_PORT = 'COM3'  # Altere para a porta serial do seu ESP32
BAUD_RATE = 115200
OUTPUT_FILE = 'mpu6050_data.txt'

def main():
    # Inicializa a conexão serial
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Conectado à porta {SERIAL_PORT}")
    except serial.SerialException as e:
        print(f"Erro ao abrir a porta serial: {e}")
        return

    # Abre o arquivo para escrita (modo 'a' para append)
    with open(OUTPUT_FILE, 'a') as f:
        # Escreve cabeçalho se o arquivo estiver vazio
        if f.tell() == 0:
            f.write("Timestamp,Roll,Pitch,Yaw\n")
        
        print("Coletando dados... Pressione Ctrl+C para parar.")
        try:
            while True:
                if ser.in_waiting > 0:
                    # Lê uma linha da serial
                    line = ser.readline().decode('utf-8').strip()
                    
                    # Verifica se a linha contém dados válidos
                    if line.startswith("Roll:"):
                        # Extrai os valores
                        parts = line.split()
                        roll = parts[1]
                        pitch = parts[3]
                        yaw = parts[5]
                        
                        # Obtém timestamp atual
                        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
                        
                        # Formata a linha para salvar
                        data_line = f"{timestamp},{roll},{pitch},{yaw}\n"
                        
                        # Escreve no arquivo
                        f.write(data_line)
                        f.flush()  # Garante que os dados são escritos imediatamente
                        
                        # Mostra no console (opcional)
                        print(data_line.strip())
                        
        except KeyboardInterrupt:
            print("\nColeta de dados encerrada.")
        finally:
            ser.close()
            print(f"Dados salvos em {OUTPUT_FILE}")

if __name__ == "__main__":
    main()