import serial
import serial.tools.list_ports
from datetime import datetime
import tkinter as tk
from tkinter import ttk, messagebox
import threading
import os

class MPU6050Logger:
    def __init__(self, root):
        self.root = root
        self.root.title("Guided Hands Data Logger")
        self.root.geometry("500x350")
        
        self.serial_conn = None
        self.logging_active = False
        self.base_filename = "dados_guided_hands"  # Nome base sem extensão
        
        self.create_widgets()
        self.update_ports()
    
    def create_widgets(self):
        # Frame principal
        main_frame = ttk.Frame(self.root, padding="15")
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # Seletor de porta serial
        ttk.Label(main_frame, text="Porta Serial:").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.port_combobox = ttk.Combobox(main_frame)
        self.port_combobox.grid(row=0, column=1, sticky=tk.EW, pady=5)
        
        # Botão para atualizar portas
        refresh_btn = ttk.Button(main_frame, text="↻ Atualizar Portas", command=self.update_ports)
        refresh_btn.grid(row=0, column=2, padx=5)
        
        # Configuração de baud rate
        ttk.Label(main_frame, text="Baud Rate:").grid(row=1, column=0, sticky=tk.W, pady=5)
        self.baudrate_var = tk.StringVar(value="115200")
        baudrate_entry = ttk.Entry(main_frame, textvariable=self.baudrate_var)
        baudrate_entry.grid(row=1, column=1, sticky=tk.EW, pady=5)
        
        # Nome base do arquivo
        ttk.Label(main_frame, text="Nome Base do Arquivo:").grid(row=2, column=0, sticky=tk.W, pady=5)
        self.filename_var = tk.StringVar(value=self.base_filename)
        filename_entry = ttk.Entry(main_frame, textvariable=self.filename_var)
        filename_entry.grid(row=2, column=1, sticky=tk.EW, pady=5)
        
        # Visualização do nome completo do arquivo
        ttk.Label(main_frame, text="Arquivo será salvo como:").grid(row=3, column=0, sticky=tk.W, pady=5)
        self.full_filename_var = tk.StringVar()
        full_filename_label = ttk.Label(main_frame, textvariable=self.full_filename_var, foreground="blue")
        full_filename_label.grid(row=3, column=1, sticky=tk.W, pady=5)
        
        # Atualiza a visualização quando o nome base muda
        self.filename_var.trace_add("write", self.update_filename_preview)
        self.update_filename_preview()
        
        # Área de status
        ttk.Label(main_frame, text="Status:").grid(row=4, column=0, sticky=tk.W, pady=5)
        self.status_var = tk.StringVar(value="Pronto para iniciar")
        status_label = ttk.Label(main_frame, textvariable=self.status_var)
        status_label.grid(row=4, column=1, columnspan=2, sticky=tk.W, pady=5)
        
        # Botões de controle
        button_frame = ttk.Frame(main_frame)
        button_frame.grid(row=5, column=0, columnspan=3, pady=15)
        
        self.start_btn = ttk.Button(button_frame, text="▶ Iniciar Captura", command=self.start_logging)
        self.start_btn.pack(side=tk.LEFT, padx=10)
        
        self.stop_btn = ttk.Button(button_frame, text="■ Parar Captura", command=self.stop_logging, state=tk.DISABLED)
        self.stop_btn.pack(side=tk.LEFT, padx=10)
        
        # Área de log
        ttk.Label(main_frame, text="Log de Atividades:").grid(row=6, column=0, sticky=tk.W, pady=5)
        self.log_text = tk.Text(main_frame, height=6, width=60, wrap=tk.WORD)
        self.log_text.grid(row=7, column=0, columnspan=3, sticky=tk.EW)
        
        # Adiciona barra de rolagem
        scrollbar = ttk.Scrollbar(main_frame, orient=tk.VERTICAL, command=self.log_text.yview)
        scrollbar.grid(row=7, column=3, sticky=tk.NS)
        self.log_text['yscrollcommand'] = scrollbar.set
        
        # Configurar expansão de coluna
        main_frame.columnconfigure(1, weight=1)
    
    def update_filename_preview(self, *args):
        """Atualiza a visualização do nome do arquivo com timestamp"""
        self.base_filename = self.filename_var.get().strip() or "dados_mpu6050"
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        full_name = f"{self.base_filename}_{timestamp}.txt"
        self.full_filename_var.set(full_name)
    
    def generate_filename(self):
        """Gera o nome do arquivo com timestamp atual"""
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        return f"{self.base_filename}_{timestamp}.txt"
    
    def update_ports(self):
        """Atualiza a lista de portas seriais disponíveis"""
        ports = [port.device for port in serial.tools.list_ports.comports()]
        self.port_combobox['values'] = ports
        if ports:
            self.port_combobox.set(ports[0])
        self.log_message(f"Portas seriais atualizadas. {len(ports)} portas encontradas.")
    
    def start_logging(self):
        """Inicia a captura dos dados"""
        port = self.port_combobox.get()
        baudrate = self.baudrate_var.get()
        self.output_file = self.generate_filename()
        
        if not port:
            messagebox.showerror("Erro", "Selecione uma porta serial!")
            return
        
        try:
            self.serial_conn = serial.Serial(port, int(baudrate), timeout=1)
            self.logging_active = True
            
            # Atualiza interface
            self.start_btn.config(state=tk.DISABLED)
            self.stop_btn.config(state=tk.NORMAL)
            self.status_var.set(f"Capturando dados de {port}...")
            
            # Inicia thread para leitura serial
            self.thread = threading.Thread(target=self.read_serial_data, daemon=True)
            self.thread.start()
            
            self.log_message(f"Captura iniciada em {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
            self.log_message(f"Dados sendo salvos em: {os.path.abspath(self.output_file)}")
            
        except Exception as e:
            messagebox.showerror("Erro", f"Não foi possível abrir a porta serial:\n{str(e)}")
            self.log_message(f"Erro: {str(e)}")
    
    def stop_logging(self):
        """Para a captura dos dados"""
        self.logging_active = False
        
        if self.serial_conn and self.serial_conn.is_open:
            self.serial_conn.close()
        
        # Atualiza interface
        self.start_btn.config(state=tk.NORMAL)
        self.stop_btn.config(state=tk.DISABLED)
        self.status_var.set("Captura encerrada")
        
        self.log_message(f"Captura encerrada em {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        self.log_message(f"Arquivo salvo em: {os.path.abspath(self.output_file)}")
        self.update_filename_preview()  # Atualiza para novo timestamp
    
    def read_serial_data(self):
        """Lê dados da porta serial e salva no arquivo"""
        try:
            with open(self.output_file, 'a') as f:
                # Escreve cabeçalho se o arquivo estiver vazio
                if f.tell() == 0:
                    f.write("Timestamp,Roll,Pitch,Yaw\n")
                
                while self.logging_active and self.serial_conn.is_open:
                    try:
                        if self.serial_conn.in_waiting > 0:
                            line = self.serial_conn.readline().decode('utf-8').strip()
                            
                            if line.startswith("Roll:"):
                                parts = line.split()
                                if len(parts) >= 6:  # Verifica se tem todos os campos
                                    roll = parts[1]
                                    pitch = parts[3]
                                    yaw = parts[5]
                                    
                                    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
                                    data_line = f"{timestamp},{roll},{pitch},{yaw}\n"
                                    
                                    f.write(data_line)
                                    f.flush()
                                    
                                    # Atualiza interface (deve ser feito na thread principal)
                                    self.root.after(0, self.update_log_display, line)
                    
                    except UnicodeDecodeError:
                        continue  # Ignora erros de decodificação
                    except Exception as e:
                        self.root.after(0, self.log_message, f"Erro na leitura: {str(e)}")
                        break
        
        except Exception as e:
            self.root.after(0, self.log_message, f"Erro ao escrever no arquivo: {str(e)}")
            self.root.after(0, self.stop_logging)
    
    def update_log_display(self, message):
        """Atualiza a exibição do log na interface"""
        self.log_text.insert(tk.END, message + "\n")
        self.log_text.see(tk.END)
        if self.log_text.index('end-1c').split('.')[0] > '50':
            self.log_text.delete(1.0, 2.0)  # Mantém o log com no máximo 50 linhas
    
    def log_message(self, message):
        """Adiciona uma mensagem ao log"""
        timestamp = datetime.now().strftime("[%H:%M:%S] ")
        self.log_text.insert(tk.END, timestamp + message + "\n")
        self.log_text.see(tk.END)
        self.log_text.update()

if __name__ == "__main__":
    root = tk.Tk()
    app = MPU6050Logger(root)
    root.mainloop()