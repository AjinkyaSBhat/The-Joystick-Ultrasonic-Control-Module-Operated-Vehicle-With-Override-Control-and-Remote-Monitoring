# The-Joystick-Ultrasonic-Control-Module-Operated-Vehicle-With-Override-Control-and-Remote-Monitoring

Welcome to the The-Joystick-Ultrasonic-Control-Module-Operated-Vehicle-With-Override-Control-and-Remote-Monitoring wiki!
 This repository contains all the code for the project.
1. Field Model Arduino code(DryvMe_Twist_Final.ino)
2. Lab Model Arduino code(DualUltraController_ROS_Final.ino)
3. Joystick conversion node (joy_2_arduino_twist.py)
4. Simple rosbridge publisher code (rosbridge_joy_ultra.html)
5. Launch file 

NOTE: Changes may have to be made in each code depending on your setup. 
1.A joystick may need to be mapped. This code was designed for the Logitec F710 Wireless.
2. Arduino movement functions may have to be tuned as per your vehicle. The core logic should remains same.
3. Ultrasonic sensor code was designed for HC-SR04 Ultrasonic Sensor for Arduino.
4. Joystick drivers for ROS must be installed before running. 
5. Launch file maps the arduinos to different names to avoid clashing of names. The ports that they are connected to may vary according to the user.
6. Rosbridge dependencies should be installed before running the server. The output of the publisher can be seen in the web console in real time.
