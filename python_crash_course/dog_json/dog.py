from json import JSONEncoder

class Dog(JSONEncoder):
    """Testing out classes with a simple dog object"""

    def __init__(self, name, age):
        """Initialize some attributes"""
        self.name = name
        self.age = age

    def sit(self):
        """ Simulate sitting"""
        print(f"{self.name} is now sitting.")

    def roll_over(self):
        """Simulate rolling over"""
        print(f"{self.name} rolled over like a boss")

    def default(self, o):
        return {self.name, self.age}
