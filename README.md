# Smart thrash bin

Code for the final project for the undergraduate course "Embedded Systems" (PMR3402) from the seventh semester of Mechatronics Engineering at Universidade de São Paulo.

Consists in a “smart trash bin” connected to an online web platform that provides an automated way of managing grocery lists in a household. 

Everytime the user throws away the package of a product, the device decrements its quantity in an online database in such way that the groceries list is always updated. Furthermore, the devices is equipped with an actuated lid that opens when approximation is detected.   

An Arduino UNO and an ESP8266 were used for the implementation.

Additionaly, a web application was developed for user interaction. The code for it can be checked here: [Merc4d0](https://github.com/victor-kowalski-m/merc4d0).

Code structure (in the *lixeira_inteligente* folder):

- *lixeira_inteligente.ino*: main file, implements a state machine and coordinates the system operation.
- *task_switcher.cpp*: implements task scheduling functionalities.
- *esp8266.cpp*: implements wi-fi communication.
- Remaining files: implement classes for each component of the system.
