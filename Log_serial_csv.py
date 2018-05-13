import os, csv, serial

ser = serial.Serial('COM3', 9600)
ser.flushInput()


csv_path = os.path.dirname(os.path.abspath(__file__)) + "\serial_log.csv"
print(csv_path)
with open(csv_path, mode='w', newline='') as file:
    writer = csv.writer(file, dialect="excel")
    while True:
        str = ser.readline().decode('utf-8')
        row = str.split()
        print(row)
        writer.writerow(row)
