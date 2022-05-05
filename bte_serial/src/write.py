import serial

mes = "messge from my Mac"
# initialize bt serial, serial port depends on the running enviroment
m5data = serial.Serial('/dev/tty.ESP32test',115200, timeout=3)
# read from bt and eliminate '\r\n'
line = m5data.readline()
print(line.decode()[:-2:])
# write to bt
m5data.write(mes.encode())
m5data.close()