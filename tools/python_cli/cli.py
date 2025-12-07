from serializer import Serializer   #this is dumb

def ask_for_port():

    while True:
        ports = ser.get_ports()
        index = 0
        for port in ports:
            print(f"Device {index}: {port.device}, VID:PID={port.vid}:{port.pid}, Serial Number: {port.serial_number}")
            index += 1
        try:
            selection = input("which device: ").strip()
            selection = int(selection)
            if selection in range(index):
                if ser.connect_port(ports[selection].device) == True:
                    print("Port opened!")
                    break
                else:
                    print("Something when wrong with opening this port")
            else:
                print("Number out of range")
        except ValueError:
            print("Invalid input")
    

if __name__ == "__main__":
    ser = Serializer(115200)
    print("hi")
    ask_for_port()
    exit = 0 
    while exit == 0:
        for i in range(5):
            ser.send("hello khoa")
        print("done sending")
        exit = input("type 1 to exit 0 to keep going").strip()
    ser.close_port()




