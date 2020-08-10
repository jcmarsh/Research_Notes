import time
import threading

sync_boolean = False

def thread_stuff():
    global sync_boolean
    while(True):
        print("\tMy thread")
        time.sleep(1)
        test_class.sync_boolean = True

class test_class():
    sync_boolean = False

    def __init__(self):

        self.my_thread = threading.Thread(target=thread_stuff)
        self.my_thread.start()


    def loop(self):
        while(True):
            time.sleep(0.1)
            if (test_class.sync_boolean):
                print("Hello")
                test_class.sync_boolean = False

def main():
    foo = test_class()
    foo.loop()


if __name__ == "__main__":
    main()

