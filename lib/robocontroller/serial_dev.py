import serial
import serial.tools.list_ports
import time
import sys
from utils.utils import Utils

utl = Utils("Serial Post")

class Serial_Dev:
    def __init__(self) -> None:
        ports = serial.tools.list_ports.comports()

        utl.INFO ("Serach Port : ", ports)

        for port in ports:
            utl.INFO ("Try to Opening Port : ", port)
            try:
                # 打开串行端口
                self.ser_dev = serial.Serial(port.device, 115200, timeout=1)
                utl.INFO(f'open port {port.device}: Successful')
            except serial.SerialException as e:
                utl.INFO(f'Could not open port {port.device}: {e}')
            
    
    def send_data (self, data) :
        if not self.ser_dev.is_open:
            print ("dose not serach serial device")
        else :
            utl.INFO(f'Sending data to {self.ser_dev.port} :  {data}')
            self.ser_dev.write(data)            

    def dev_close (self):
        self.ser_dev.close()

if __name__ == "__main__" :
    ser = Serial_Dev()
    ser.send_data([0x02])
