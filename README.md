# The Simple Parameter File Reader - SPFR for C++
Introduction
------------

Because there are no simple reader for parameter files (or rather I can't find them), I created this small project.
It comes without any other dependencies, except for the the fact that I created and tested this code on a Ubuntu 20.04
x86_64 platform. In the test folder you can find a example on how to use the library. I got the inspiration from the 
ROS project and its "getParam", but I wanted something smaller just for my private needs.

Overview
--------

This project contains the readConfig.cpp and readConfig.hpp for the library and a test.cpp to show in a executable the
usage of the library. For my needs the implemented functions are adequate, but it would be easy to adept the functions
to read additional data types. 

Requirements
------------

I wrote, compiled and tested the project on a Ubuntu 20.04 machine with amd64 architecture. There are no additional
packages required, only a fresh installation of the OS.

Usage
-----

Normally you want your configuration file somewhere in your project. Because of changing paths or different name 
conventions I chose to copy the configuration file to the build folder with the help of the CMakeList.txt or rather
the cmake tool. This way the executable can always find the right file. You can have a look in the CMakeList.txt
for a better understanding.

The usage of the library is pretty easy. You call the object with the path to the configuration file and afterwards 
you can read from it with:

	$ OBJECT_NAME.get_parameter("NAME_OF_THE_PARAMETER", VARIABLE);
	
I chose to have a initialized variable, because I like the idea that even if things go wrong or you change your 
program or config file without caring about all parameter, the program still continues and there will be no shut
down, only because of some misread parameter.

Example of a parameter file
---------------------------

	string_example: /home/USERNAME/workspace/blubb       # Example for a comment
	character: z
	number_int: 12
	number_float: 4234.234234
	number_bool: 1
	word_bool: true
	character_array: [a, b, c, d, e, f, g]
	number_int_array: [3425,121,999999, 5555, 1234]
	number_float_array: [2.234,3234.21314234,4.5]
	string_array: {Hello, World, !}


Change Log
----------
|  Version  |     Date    |     Changes    |
|-----------|:-----------:|---------------:|
|    1.0    | 03.11.2020  | Initial commit |


Credits
-------

Author:  Andre Alexander Pieper.

Version 1.0

Date:  03.11.2020

License: LGPLv3
