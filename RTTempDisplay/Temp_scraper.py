# -*- coding: utf-8 -*-
"""
Created on Sat May  4 14:31:24 2024

@author: Jameson Giannattasio
"""
import websocket
import time
from bs4 import BeautifulSoup
from urllib.request import urlopen
from time import gmtime, strftime
while(True):
    tempurl = "https://weather.com/weather/today/l/69188156eaeabff567255aac810e1f23f792002a768283abb1b5ad4dea8823bf"
    temppage = urlopen(tempurl)
    temphtml = temppage.read().decode("utf-8")
    tempsoup = BeautifulSoup(temphtml, "html.parser")
    temperature_elements = tempsoup.find_all(attrs={"data-testid": "TemperatureValue"})
    
    current_temp = (temperature_elements[0].get_text()).strip("°")
    high_temp = (temperature_elements[1].get_text()).strip("°")
    low_temp = (temperature_elements[2].get_text()).strip("°")
    hour=time.localtime()[3]
    minutes=time.localtime()[4]
    
    if(hour>12):
        hour=hour-12
    print("Time: ", end="")
    print(hour,end="")
    print(":",end="")
    print(minutes,end="\n")
    print("Current Temp: "+current_temp)
    print("High Temp: "+high_temp)
    print("Low Temp: "+low_temp)

    ws = websocket.WebSocket()
    ws.connect("ws://10.0.0.209/test")
         
    ws.send(current_temp)
    ws.send(high_temp)
    ws.send(low_temp)
    ws.send(str(hour))
    ws.send(str(minutes))
    
         
    ws.close()
    time.sleep(10)

"""timeurl="https://www.timeanddate.com/time/zones/edt"
timepage = urlopen(timeurl)
timehtml = timepage.read().decode("utf-8")
timesoup = BeautifulSoup(timehtml, "html.parser")
time_elements = timesoup.find(id="hourmin0")
time_ampm = timesoup.find(id="ampm0")"""
"""    times=(time_elements.get_text()).split(":")
    hour=times[0]
    minutes=times[1]"""
""