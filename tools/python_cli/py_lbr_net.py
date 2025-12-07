from enum import Enum

MAX_RECEIVE_BUF_SIZE = 1028
CHECKSUM_SIZE = 1
CHECKSUM_BITS = 256
START_BYTE_SIZE = 1
ADDRESS_SIZE = 1
DATA_LEN_SIZE = 1
PACKET_HEADER_SIZE = (CHECKSUM_SIZE + START_BYTE_SIZE + ADDRESS_SIZE + DATA_LEN_SIZE)

class State(Enum):
    IDLE = 0
    FAIL = 1
    ACKNOWLEDGED = 2
    READ_ADDRESS = 3
    READ_LEN = 4
    READ_DATA = 5
    VALIDATE = 6
    FINISHED = 7

class ControlFlags(Enum):
    START_TRANSMISSION = ord('!')
    ACK = ord('+')
    NACK = ord('-')

class Lbr_net():
    def __init__(self, address):
        self.address = address
        self.state = State.IDLE
        self.receive_buffer = []
        self.sum = 0
        self.receive_index = 0
        self.package_size = None


    def pack(self, buffer, buffer_size, target, data): #do i need data size?) : data should be a INT
        index = 0
        sum = 0
        output = buffer
        data_size = len(data)
        if data_size + PACKET_HEADER_SIZE >  MAX_RECEIVE_BUF_SIZE or data_size + PACKET_HEADER_SIZE >  buffer_size:
            return False
        output[index] = ControlFlags.START_TRANSMISSION
        index += 1
        sum += ControlFlags.START_TRANSMISSION
        output[index] = target
        index += 1
        sum += target
        output[index] = data_size
        index += 1
        sum += data_size
        for ele in data:
            output[index] = ele
            index += 1
            sum += ele
        output[index] = sum % CHECKSUM_BITS
        return True
        

    def read_byte(self, data):  #data should be a int
        if self.state == State.IDLE:
            if data == ControlFlags.NACK.value:
                self.state = State.FAIL
            elif data == ControlFlags.START_TRANSMISSION.value:
                self.state = State.READ_ADDRESS
                self.receive_buffer = []
                self.sum = 0
                self.receive_index = 0  #dont know if this is needed
                self.receive_buffer.append(data)
                self.sum = data
                self.receive_index += 1
            elif data == ControlFlags.ACK.value:
                self.state = State.ACKNOWLEDGED
        elif self.state == State.READ_ADDRESS:
            if data == self.address:
                self.receive_buffer.append(data)
                self.sum += data
                self.receive_index += 1
                self.state = State.READ_LEN
            else:
                self.state = State.IDLE
        elif self.state == State.READ_LEN:
            self.package_size = data
            self.receive_buffer.append(data)
            self.sum += data
            self.receive_index += 1
            self.state = State.READ_DATA
        elif self.state == State.READ_DATA:
                self.receive_buffer.append(data)
                self.sum += data
                self.receive_index += 1
                if len(self.receive_buffer) == self.package_size + 3:
                    self.state = State.VALIDATE
        elif self.state == State.VALIDATE:
            self.sum = self.sum % CHECKSUM_BITS
            if self.sum != data:
                self.state = State.FAIL
                self.receive_buffer.append(data)
                self.receive_index += 1
            else:
                self.state = State.FINISHED
                self.receive_buffer.append(data)
                self.receive_index += 1
        return
                


    def flush_data(self, buffer: list[int]) -> None:
        if self.state == State.FINISHED:
            buffer[:] = self.receive_buffer[3:3 + self.package_size]
        else:
            buffer.clear() 
        self.package_size = 0
        self.receive_buffer = []
        self.sum = 0
        self.state = State.IDLE
        return
            

    def get_package_size(self):
        return self.package_size

