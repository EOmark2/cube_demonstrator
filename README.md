# Cube Demonstrator
Cube Demonstrator for Cooperative Driving. With this cube it is possible to send maneuvers to a tcp/ip server. 

The prototype is built like a cube, which can be moved by the angles: pitch and roll, while the angle in which the cube is held is calculated. Thereby a maneuver can be executed, which is sent over the protocol TCP to a server. The server processes the signal and displays the desired maneuver in a GUI. For the implementation of the cube, a CAD file was first designed to print it out with a 3D printer. Then an ESP32 was equipped with an acceleration sensor and an addressable led strip. Based on this, the code to be executed was implemented in C++ and the TCP server was declared with Python.
