import serial

# bluetooth serial communication test program using the M5stack buttons
# simply receive the message and display
# initialize bt serial, serial port depends on the running enviroment
m5data = serial.Serial('/dev/tty.ESP32test',115200, timeout=3)

while True:
    # read from bt and eliminate '\r\n'
    line = m5data.readline().decode()[:-2:]
    if line != "":
        print(line)
        if line == "t":         # if the button "b" was pushed, terminate the script
            break
    
m5data.close()
