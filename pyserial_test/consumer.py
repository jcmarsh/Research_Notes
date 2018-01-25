from serial import Serial


print("Ello!")

serial_con = Serial(port='/dev/ttyUSB1', baudrate=460800, timeout=15, rtscts=True)

counter = 0
while True:

    counter = serial_con.inWaiting()
    if counter:
        char = serial_con.read(counter).decode('utf-8')
        print("\nInWaiting: ", counter)
        print(char, end='')

print("Done.")

