from serial import Serial

print("Ello!")

serial_con = Serial(port='/dev/ttyUSB1', baudrate=460800, timeout=15, rtscts=True)

while True:
    char = serial_con.read()
    dec_char = char.decode('utf-8', 'replace').replace('\x00', 'X')

    print(dec_char, end="")

print("Done.")

