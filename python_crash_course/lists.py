print("Lists dude.")

cars = ['prius', 'sunfire', 'camry', 'ranger']

print(cars)

cars.append('prius')

print(cars)

cars.remove('prius')

print(cars)

# Trying out a comprehension <- wow, that is easy
squares = [value ** 2 for value in range(1,11)]

print(squares)
