import serial
import csv

arduino_port = "COM3"           # serial port of Arduino
baud = 9600                     # arduino uno runs at 9600 baud
fileName="data.csv"             # name of the CSV file generated
ser = serial.Serial(arduino_port, baud)
print("Connected to port:" + arduino_port)

TIME_CAPTURED_MS = 30000        # total time to capture
DELAY_BETWEEN_SAMPLES = 13

# how many samples to collect
SAMPLES = TIME_CAPTURED_MS/DELAY_BETWEEN_SAMPLES

line = 0
data = []                       # store data

# collect the samples
with open(fileName, "a", encoding='UTF8') as file:
    print("Created file")
    while line <= SAMPLES:
        getData=ser.readline()
        dataString = getData.decode('utf-8')
        data=dataString[0:][:-2]
        #print(data)
        readings = data.split(",")
        #print(readings)
        data.append(readings)
        #print(data)

        line += 1

# create the CSV
with open(fileName, 'w', encoding='UTF8', newline='') as f:
    writer = csv.writer(f)
    writer.writerows(data)

print("Data collection complete!")
file.close()
ser.close()
