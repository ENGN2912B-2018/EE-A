# Embedded Spatial Tracking with Kalman Filtering
<p align="center">
  <img src="https://www.brown.edu/academics/engineering/sites/brown.edu.academics.engineering/themes/engineering/img/brown-engineering-logo.png" alt="Brown School of Engineering Logo"/>
</p>

## Embedded Electronics, Group A
Team: Kevin Anderson (kevin_anderson@brown.edu, Michael Caplan (michael_caplan@brown.edu), Yale Friend (yale_friend@brown.edu), Evan Pandya (evan_pandya@brown.edu)

Organization: Brown University, ENGN2912B: Scientific Programming in C++

Date(s) of Submission: 12/16/18 (Code repository), 12/18/18 (Completed README.md)

## Introduction
This documentation should provide 1) an introduction and technical overview of the project for new users, 2) detailed step-by-step instructions on how to build the project on the CCV (or other platform, as appropriate), and 3) instructions explaining how to run your program with example input and expected output.
Your build instructions must list any dependencies:  What software libraries and versions are required to run?  For the CCV, include your .modules file in the repository so it can simply be loaded before compiling with CMake and GCC.
## Project Overview
## Goals and Objectives 
## Software Architectural Design
#### Required External Libraries
#### Functional Block Diagrams 
#### Data Flow Diagrams
#### UML Diagrams showing Object Definition/Inheritance
## Interface Descriptions
#### Graphical Interface/Data Visualization 
In order to graphically display the position data before and after Kalman filtering, we created a Graphical User Interface in Qt Creator version 5.10.1. Our GUI allows the user to read in a .csv file containing raw, 3 dimensional position data (units: m), specify the sampling rate, and display the axial (x, y, or z, based on user selection) position of the tracked object against time (unit: s). In addition to te aforementioned 2-dimensional plots, our data visualization application can also generate a 3-dimensional scatter plot that shows the x, y, and z points in space to give the user a sense for where the object has moved over the course of the movement trial. Detailed instructions for running our GUI can are located under the "Instructions for Compiling and Running the Software" section of the README. 
#### Communication Protocols 
#### Threading and Concurrency
#### Exception Handling
## Testing and Evaluation 
#### OS and Compiler/Library Versions Tested
#### Description of Functional Testing to date
#### Instructions for Compiling and Running the Software
## Conclusions
Discuss major accomplishments, results of the software development efforts, and answer:  Were you successful in meeting all goals and objectives?  What didn't work?  What would take more time?
#### Future Work
Description of potential future software expansions and software features
#### Author Contributions 
| Software contribution       | Author          | 
| ------------- |:-------------:|
| a      | a | 
| b      | b      |  
| c | c      |  
| d |d






### Motivation
Tracking spatial position has become increasingly cheap over the years with the advent of satellite positioning, most recognizably exemplified by the widespread Global Positioning System. However cheap and compatible they are, GPS systems are not simple electronic devices. Our project seeks to implement robust positioning by applying Kalman filtering on data from a MEMS accelerometer on a Raspberry Pi.

### Libraries
- _**SPI**_ - C-Periphery
- _**FFT**_ - FFTW
- _**Visualization**_ - VTK and Qt
- _**Kalman Filtering**_ - Eigen, SourceForge "Kalman Filter"

### Applications
Biking Tracking and Building Sway
