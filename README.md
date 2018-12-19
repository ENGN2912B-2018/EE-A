# Embedded Spatial Tracking with Kalman Filtering
<p align="center">
  <img src="https://www.brown.edu/academics/engineering/sites/brown.edu.academics.engineering/themes/engineering/img/brown-engineering-logo.png" alt="Brown School of Engineering Logo"/>
</p>

## Embedded Electronics, Group A
**Team:** Kevin Anderson (kevin_anderson@brown.edu), Michael Caplan (michael_caplan@brown.edu), Yale Friend (yale_friend@brown.edu), Evan Pandya (evan_pandya@brown.edu)

**Organization:** Brown University, ENGN2912B: Scientific Programming in C++

**Date(s) of Submission:** 12/16/18 (Code repository), 12/18/18 (Completed README.md)

## Introduction
At a high level, this software package provides two complementary tools. First, we provide a software package targeted to run on a Raspberry Pi connected to an ADXL345 accelerometer. This will allow the user to input a number of samples to be collected from the accelerometer. The accelerometer runs at 3200 Hz and each sample is collected in an internal 32 level FIFO before being read by the Raspberry Pi over SPI. This software runs the samples through a Kalman filter and then performs numerical integration to mitigate error and derive velocity and position from acceleration data. Our software then outputs the raw data collected from the accelerometer and the Kalman filtered data into two separate .csv files. The files can then be loaded into the GUI and visualized for easy analysis. 

## Project Overview
#### Motivation 
Tracking spatial position has become increasingly cheap with the advent of more universally accessible methods of satellite positioning; however, regardless of how cheap and compatible they may be, GPS systems are fundamentally complex electronic devices. As a result, they are *relatively* much more expensive and power consumptive than simple MEMS devices like accelerometers. A current GPS module like the Gms-u1LP that advertises low power consumption averages about 24 mA of current usage while GPS tracking is active. This is because the GPS unit must be in constant communication with satellites over a low bitrate link, necessitating extended periods of antenna power. In contrast, an industry standard accelerometer like the ADXL345 averages around 23 µA of current usage--a reduction in power by three orders of magnitude. This power save can make an incredible difference in improving embedded applications that seek to optimize the relatively limited energy density of modern battery technology.  
#### High-level project overview
In this project, we apply Kalman filtering techniques on ADXL345 accelerometer data to perform spatial position tracking on a low-power embedded system, namely a Raspberry Pi. 

## Goals and Objectives
+ Estimate the position of system 
+ Generate user-friendly interactive visualization
+ Develop Kalman filter with efficient C++ library (Eigen)
+ Establish SPI communication between Raspberry Pi and Accelerometer

## Software Architectural Design
#### Required External Libraries
- _**SPI**_ 
- _**BCM2835**_
- _**pthread**_  
- _**Qt Creator**_ + _**Qt Data Visualization Library**_
- _**QCustomPlot**_ 
- _**Eigen**_
- _**Liquid SDR**_ *(deprecated)*  
#### Functional Block Diagrams 
[Software Architecture Block Diagram](images/block_diagram.PNG)
#### Data Flow Diagrams
[Software Architecture Data Flow Diagram](images/data_flow.PNG)
#### Interface Descriptions
The ADXL345 class is responsible for interfacing with the accelerometer.
There are 3 main public methods:

| Method   | Description   |
| ------------- |:-------------:|
| float* ADXL345::calibrate(int n) | This returns an array of length 3, which contains a time average over *n* samples of each axis.  |
| void ADXL345::start() |  This sets a start flag which starts the queue in a separate thread and starts collecting samples from the accelerometer over SPI. |
| float** ADXL345::read(int n) | This method can be used in 2 ways. First, you can set *n* to be the length of the queue, and then collect all your samples after the queue fills up. Second, you can set *n* to be less than the length of the queue and pop data off the queue in chunks. This is the way that we use the method in spatial.cpp: we read in chunks of 1, so that each data point can be filtered and integrated before moving to the next. |

#### Installing Kalman Filter/Accelerometer Interface
To install the software on the raspberry pi:

```
 git clone https://github.com/ENGN2912B-2018/EE-A/  
 cd EE-A  
 mkdir build; cd build  
 cmake ..  
 make  
```
This will output an executable called spatial, which can be run by
```
 sudo ./spatial  
```
Note: sudo is important, otherwise you will get a "segmentation fault" error.

For installing the GUI, see below:
#### Graphical Interface/Data Visualization 
In order to graphically display the position data before and after Kalman filtering, we created a Graphical User Interface (GUI) in Qt Creator version 5.10.1. Our GUI allows the user to read in a .csv file containing raw, 3 dimensional position data (units: m), specify the sampling rate, and display the axial position (x, y, or z, based on user selection) of the tracked object against time (unit: s). In addition to te aforementioned 2-dimensional plots, our data visualization application can also generate a 3-dimensional scatter plot that shows the x, y, and z points in space to give the user a sense for where the object has moved over the course of the movement trial. A picture of the GUI can be found here: [Sample GUI Image](images/gui_image.PNG). Detailed instructions for running our GUI can are located under the "Instructions for Compiling and Running the Software" section of the README. 
#### Communication Protocols 
Communication with the ADXL345 is done over 4-wire SPI using the BCM2835 library. There are 4 types of SPI protocols, each of which signifies the clock polarity and phase used to interpret data. The ADXL345 uses SPI mode 3, which means that clock polarity and phase are both mode 1. We originally were using the WiringPi library, however, we discovered that it is only capable of running SPI mode 1 which is not suitable for this project. We write to a few registers on the ADXL345 that in turn sets the data rate, the bit order for data transfer, and begins sample collecting in the 32 level FIFO. After this, we read 6 bytes concurrently and the accelerometer sends back instantaneous data for 3 separate axes: x, y, and z. Each of these axes is represented by 2 bytes of data containing a 10 bit sign extended two’s complement integer. This integer was then converted to a float and multiplied by a scaling factor of 3.9 m*g*/LSB, where *g* is 9.8 m/s/s. 
#### Threading and Concurrency
Threading was necessary because we want to perform data processing at the same time as data collection. Otherwise, if we did not thread,  we could risk dropping samples, leading to data discontinuities that could cause system-wide errors. In order to do this, we used a lockless, wait-free, circular FIFO. This code was courtesy of Kjell Hedstrom (https://bitbucket.org/KjellKod/lock-free-wait-free-circularfifo/src). Specifically, this data structure is suitable for a single-producer, single-consumer multithreaded queue, which fit perfectly into our implementation. This type of queue was written for real-time audio processing, so it was necessary that pushing and popping could be executed expediently. The std::thread class (introduced in C++11) was used to spawn a thread with a worker that read SPI and pushed back data to the queue. Then, the main thread of the program pops data from the queue, applies the Kalman filter, and numerically integrates.
#### Exception Handling
In order to make our system more robust, we implemented exception handling in our graphical user interface. The GUI checks to make sure that the .csv file that the user inputs is valid and if not, prompts the user to input a different one. We also designed the GUI so that the user must write a valid .csv file name, choose a sampling rate, and specify which axis to display *before* plotting occurs. 
## Testing and Evaluation 
#### OS and Compiler/Library Versions Tested
+ GCC (4.8 or later)
+ Linux (Raspbian and Debian)
+ Windows 10 (Kalman)
#### Description of Functional Testing to date
We generated MATLAB scripts to quickly double-check the visualization of reported accleration data for different state sizes. We also wrote a MATLAB script that performs the numerical integration on filtered acceleration data using built-in functions for comparative purposes. The MATLAB testing scripts can be found in the repository under a folder named *MATLAB*.
#### Instructions for Compiling and Running the Software
###### Graphical Interface/Data Visualization
Our GUI was created using Qt Creator version 5.10.1 and made use of the QCustomPlot external library and the Qt Data Visualization module that comes with this version of the software. Everything was created and tested on the CCV; our executable file will run on any Linux machine, but will not on Windows and iOS devices. The instruction list below provides a comprehensive tutorial on how to run and build our GUI software application **in the CCV environment**: 
1. From the home directory, run the command *cd /gpfs/runtime/opt/qt/5.10.1/Tools/QtCreator/bin* to go to the directory where you may enter the Qt Creator environment through the command *./qtcreator*
2. Once the IDE opens, you will be taken to the Welcome screen where you can select the *Open Project* button. After you have downloaded our code, navigate to the directory it was downloaded to and go into the folder titled "EE_A_Data_Vis." Now select the available .pro file of the same name to open the project. 
3. From here, click build on the bottom left of the IDE and wait a few minutes for the files to be auto-generated by Qt. 
4. Click run and the GUI should pop up. From here, you may specify a .csv file (it must be in the same directory as the .pro and other files associated with the project) to read in, select a sampling rate, and then click the plot buttons to see 2D and 3D position data. 

## Conclusions
The main objectives were mostly achieved. Through numerical integration, we were able to successfully determine the position of the system within a 0.5 m deviation from the true position. Consequently, we were also able to provide estimates on the velocity of the system. Lastly, we developed an intuitive and user-friendly GUI using Qt, plotting three-axis data for analysis in both two and three-dimensional space. 

Applying a Kalman Filter to acceleration data to directly find position proved to be extremely difficult. Regardless of the tuning (the adjusting of the values in the covariance matrix or adjustment to terms in the equations), we could not develop a system which gave consistent position data for even the most simplistic tests (e.g. no motion, motion in one direction, etc.). As our end goal was to track position, we realized we had to change our methods. Rather than use the Kalman filter to get position data directly, we decided to use numerical integration to get velocity *first*, and then get position next. The Kalman filtering was done just on the acceleration data. 

Another consequence of the accelerometer was that angles could not be derived from the data. This is because two axes of tilt make the system underdetermined. To account for this, we had to constrain the tilt of the accelerometer such that the z-axis pointed directly down. Even with this added constraint, components from gravity affected the acceleration reports in the x and y axes which resulted in slight errors.


#### Future Work
The ADXL345 accelerometer contains many features that we did not take advantage of during this project. One certainly useful capability is the “Self Test” feature, which automatically calibrates the axes. During testing we set the accelerometer at the highest sensitivity, which happens right at the beginning in the configuration of the module. To make the the positional tracker’s applications more flexible, we could make the desired sensitivity a user input at the front end. 

As stated above, we had to constrain one axis of rotation in order to successfully track position. In order to track position with no rotational constraints, we would need to add another external sensor, such as a gyroscope or magnetometer. These sensors would give us the necessary data to recover the rotational frame. 

Our Kalman filter relied on a linear relationship on the data to be processed. The Extended Kalman Filter can better handle non-linear relationships and therefore would produce more robust error correction. 

Perhaps our biggest future goal is the data pipeline. In our current implementation, data is accumulated, processed, and stored on the Pi. Those files then must be separately transferred to the host machine for the GUI. This process works fine if the user does need real-time positional tracking and just needs historical data. To implement real-time tracking, we would need to add in a Mobile Data HAT (Hardware Attached on Top) to stream the data from the Pi to the GUI host machine (assuming the tracker isn’t in a WiFi environment—the RPi 3B has built-in WiFi). We would then need to modify our GUI to accommodate real-time data, which can be done within QCustomPlot, which is already used in the project. 


#### Author Contributions 

| Software contribution       | Author          | 
| ------------- |:-------------:|
| Evan Pandya | GUI | 
| Yale Friend | GUI & Embedded Software |  
| Kevin Anderson | Kalman Filtering & Numerical Integration |  
| Michael Caplan | Embedded Software & Accelerometer Communication |




