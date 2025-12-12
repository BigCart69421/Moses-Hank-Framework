**Moses R2 Template** <br>
Ths folder contains the baseline Moses R2 board, and code examples
The Moses R2 template consist of:
+ A Arduino Nano or Arduino Uno OR Raspberry Pi Pico
+ One Servo Motor (D9)
+ A BMP180 over I2C
+ A LED Light
+ NO PYRO CIRCUIT

The code stores the variables on RAM before flashing to the SD Card, and is limited to 150 samples at 1hz. On the pico, these values could be jumped up for more frequency and more samples.
