from lib.robocontroller.serial_dev import Serial_Dev
from utils.utils import Utils
import time
import numpy as np

utl = Utils("RoboController")

class RoboController: 
    def __init__(self) -> None:
        self.ser_dev = Serial_Dev()
        # Test Serial Link
        Test_Serial_Link = False
        if Test_Serial_Link:
            data = bytearray([0xFF, 0xFF, 0x03, 0x01, 0x32, 0xD7])
            self.ser_dev.send_data(data)
            time.sleep(2)
            data = bytearray([0xFF, 0xFF, 0x03, 0x01, 0x00, 0xD7])
            self.ser_dev.send_data(data)

    
    def move (self, x, y, theta, Dy = 0.25, Dx = 0.25, C = 0.05 * np.pi * 2) :
        utl.INFO(f"MOVE: x:{x} m, y:{y} m, theta:{theta} rad")
        d1 = x - y - theta * (Dx + Dy)
        d2 = x + y + theta * (Dx + Dy)
        d3 = x + y - theta * (Dx + Dy)
        d4 = x - y + theta * (Dx + Dy)
        d = np.array([d1, d2, d3, d4])
        d /= C
        utl.INFO(f"Calc : RF: {d[0]}, LF: {d[1]}, RB: {d[2]}, LB: {d[3]}")
        self.send_rnd (0x01, d[0])
        self.send_rnd (0x02, d[1])
        self.send_rnd (0x03, d[2])
        self.send_rnd (0x04, d[3])
    

    def Mec_Velocity (self, vx, vy, omega, Dy = 0.25, Dx = 0.25, R = 0.05) :
        utl.INFO(f"Mec_Velocity: xx:{vx} m/s, xy:{vy} m/s, theta:{omega} rad/s")
        d1 = vx - vy - omega * (Dx + Dy)
        d2 = vx + vy + omega * (Dx + Dy)
        d3 = vx + vy - omega * (Dx + Dy)
        d4 = vx - vy + omega * (Dx + Dy)
        d = np.array([d1, d2, d3, d4])
        d /= R
        utl.INFO(f"Calc : RF: {d[0]}, LF: {d[1]}, RB: {d[2]}, LB: {d[3]}")
        self.send_vel (0x01, d[0])
        self.send_vel (0x02, d[1])
        self.send_vel (0x03, d[2])
        self.send_vel (0x04, d[3])





















        
    
    def send_vel (self, add, vel) :
        # 0xff 0xff 0x01 0x0a 0xaa 0xaa 0xbb
        data = [0xff, 0xff, 0x00, 0x03, 0x00, 0x00, 0x00]

        data[2] = add
        irnd = int(vel)
        highbit = (irnd & 0xff00) >> 8
        lowbit = (irnd & 0xff)

        data[4] = highbit
        data[5] = lowbit

        checkbit = (0xff + 0xff + add + 0x06 + highbit + lowbit) & 0xff

        data[6] = checkbit
        bdata = bytearray(data)
        self.ser_dev.send_data(bdata)
        utl.INFO ("data (Send_Vel rpm): ", str(data))
    
    def send_rnd (self, add, rnd) :
        # 0xff 0xff 0x01 0x0a 0xaa 0xaa 0xbb
        data = [0xff, 0xff, 0x00, 0x06, 0x00, 0x00, 0x00]

        data[2] = add
        irnd = int(rnd * 100)
        highbit = (irnd & 0xff00) >> 8
        lowbit = (irnd & 0xff)

        data[4] = highbit
        data[5] = lowbit

        checkbit = (0xff + 0xff + add + 0x06 + highbit + lowbit) & 0xff

        data[6] = checkbit
        bdata = bytearray(data)
        self.ser_dev.send_data(bdata)
        utl.INFO ("data (Send_Rnd): ", str(data))

    def close(self):
        self.ser_dev.dev_close()

if __name__ == "__main__":
    rc = RoboController()
    rc.send_rnd(0x01, 18)
    rc.close()


