import json
from dog import Dog

jake = Dog("jake", 13)

jake.sit()

print(f"Look at my dog {jake.name}!")

# Try to open an existing dog file
filename = 'saved_dog.json'
try:
    with open(filename, 'r') as f:
        print("Do the thing then")
except FileNotFoundError:
    print("No file. NBD")

with open(filename, 'w') as f:
    json.dump(jake, f)
