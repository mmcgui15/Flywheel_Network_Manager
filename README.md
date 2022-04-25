# Flywheel_Network_Manager

Software designed to manage a network of flywheels intended to store excess grid energy.


Front end implemented using Dear ImGui.
To run this program, first clone the Dear Imgui repository at https://github.com/ocornut/imgui. 
Add Flywheel_Network_Manager files as dependencies, replacing ImGUI's main.cpp with the one in this repo.


The network manager is designed to handle multiple flywheels, though it is currently set up to handle only 
a single tachometer, which will display the angular velocity of the current active flywheel. 
By default, the sensor input is turned off, which can be toggled on the flywheel settings menu when the program is running.
When toggled off, a slider will appear that can be used to adjust the velocity instead of using the actual reading.
To get the tachometer designed in this repo running, upload Tachometer_v2 to your microcontroller and set up the proper port
in Tachometer.cpp (COM3 by default).

The program is designed to be initialized by a file titled "flystore.fly". This file will be created the first time the program is closed.
"flystore.fly" is a file containing data members of all the flywheels that are part of the network.

To add a flywheel, go to "add flywheel" under "flywheel menu". From there an active flywheel window will pop up from which you can adjust
all of the parameters of that flywheel and see the sensor readings and power output.
