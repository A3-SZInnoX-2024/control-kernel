import serial
import serial.tools.list_ports
import time
import sys

def send_data():
    # 定义要发送的数据
    data = bytearray([0xFF, 0xFF, 0x01, 0x03, 0x01, 0x32, 0xD7])

    # 列出所有可用的串口
    ports = serial.tools.list_ports.comports()

    for port in ports:
        try:
            # 打开串行端口
            ser = serial.Serial(port.device, 115200, timeout=1)

            # 每10秒发送一次数据
            print(f'Sending data to {port.device}...')
            while True:
                data = bytearray([0xFF, 0xFF, 0x01, 0x03, 0x01, 0x32, 0xD7])
                ser.write(data)
                time.sleep(5)
                data = bytearray([0xFF, 0xFF, 0x01, 0x03, 0x01, 0x00, 0xD7])
                ser.write(data)
                time.sleep(5)

            # 关闭串行端口
            ser.close()
        except serial.SerialException as e:
            print(f'Could not open port {port.device}: {e}')


if __name__ == "__main__":
    print(sys.version)
    send_data()