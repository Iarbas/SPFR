# The Simple Parameter File Reader - SPFR for C++
Because there are no simple reader for parameter files (or rather I can't find them), I created this small project.
It comes without any other dependencies, except for the the fact that I created and tested this code on a Ubuntu 20.04
x86_64 platform. In the test folder you can find a example on how to use the library. I got the inspiration from the 
ROS project and its "getParam", but I wanted something smaller just for my private needs.

Here the example of how a configuration file should look like:

&&&&&&&&&&&&&&& Example &&&&&&&&&&&&&&&
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
&&&&&&&&&&&&&&& Example &&&&&&&&&&&&&&&

------------------------------------------------
------------------ Change Log ------------------
------------------------------------------------
Version |   Date   |           Changes         |
________|__________|___________________________|
        |          |                           |
  1.0   |03.11.2020|       Initial commit.     |
________|__________|___________________________|
------------------------------------------------

Author:  Andre Alexander Pieper.

Version 1.0

Date:  03.11.2020

License: LGPLv3
