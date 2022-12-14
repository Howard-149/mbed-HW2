# Mbed Homework 2

## Introduction

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
This program allows **DISCO-L4S5I** to read sensor values and send datas to server through wifi. 
Server will recieve the data and visualize them.
After import the file to mbed, there're still few steps to do to run the program:
1. Replace ```target.json``` to the origin one in ```mbed-os/targets/targets.json```
2. Modify line 14, 15 of ```mbed_app.json``` by simply filling in your own wifi SSID and password for the device to connect. 
3. Modify line 27, 28 in server.py, fill in your IP address and port. 
4. Modify line 9, 10 of ```source/main.cpp```, fill in the IP address and port of your server. 
5. Use ```python3 server.py``` to run server.py on the terminal and run the program on DISCO-L4S5I.

<!--
Use our targets.json to replace the origin one in mbed-os/targets/targets.json.
To run the program, first modify line 14,15 of mbed_app.json by simply fill in your own wifi SSID and password. 
Second, modify line 27,28 in server.py, fill in your IP address and port. 
Third, modify line 9,10 of source/main.cpp, fill in the IP address and port of your server. 
Lastly, use ```python3 server.py``` to run server.py on the terminal and run the program on DISCO-L4S5I.
-->


## Result
### Server
- **ax**: acceleration in x-direction
- **ay**: acceleration in y-direction
- **az**: acceleration in z-direction
- **gx**: angular acceleration in x-direction
- **gy**: angular acceleration in y-direction
- **gz**: angular acceleration in z-direction
<img src="https://i.imgur.com/uozoyLA.png" alt="Alt text" title="result">

### Device
<img src="https://i.imgur.com/5qJFAPB.png" alt="Alt text" title="CMD_RESULT">
