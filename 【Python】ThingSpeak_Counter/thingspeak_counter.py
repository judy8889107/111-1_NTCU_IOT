#!/usr/bin/python

import requests
import time
api_key='L2IEPI40VQX52HZV'

def sender():
    for x in range(10):
        msg=requests.get("https://api.thingspeak.com/update?api_key="+api_key+"&field1="+str(x)) # int to string
        print("\nThe Message sent was: \n\n"+str(msg))
        print ("x: ", str(x))
        time.sleep(10)

def main():
  sender()

if __name__ == '__main__':
  main()