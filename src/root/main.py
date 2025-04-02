import sys
import os

#debug
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(SCRIPT_DIR)

print("Updated sys.path:", sys.path)
#debug

from application import Application

def main():
    application = Application()
    application.run()

if __name__ == "__main__":
    main()
