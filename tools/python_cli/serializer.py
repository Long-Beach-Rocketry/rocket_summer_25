import serial.tools.list_ports
from typing import List

class Serializer:
    def __init__(self, baudrate: int, ) -> None:    #may need to add more parameters for the serial class
        self.ser = serial.Serial()
        self.ser.baudrate = baudrate
        self.ports = None
        return

    def get_ports(self) -> List[serial.tools.list_ports_common.ListPortInfo]:
        ports = serial.tools.list_ports.comports()
        self.ports = ports
        return ports 
    
    def connect_port(self, port: str) -> bool:
        if self.ser.is_open:
            self.ser.close()
        self.ser.port = port
        self.ser.open()
        return self.ser.is_open

    def close_port(self) -> bool:
        self.ser.close()
        self.ser.port = None
        return True


    def send(self, msg: str) -> bool:
        self.ser.write(msg.encode('utf-8'))
        return True

    def receive(self) -> str:

        pass

