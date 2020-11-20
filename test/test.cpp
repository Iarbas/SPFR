// ========================================== max. 120 symbols in one line ============================================
/**
 * @par Project
 * SPFR - Simple Parameter File Reader
 *
 * @file    readConfig.cpp
 * @author  Andre Alexander Pieper
 * @version 1.0
 * @date    2020-11-03
 *
 * @brief   Read the configuration file.
 *
 * This test program reads the data from the example configuration file and prints the results.
 */
// --------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include "cmake_config.h"
#include "readConfig.hpp"


using namespace std;

int main (int argc, char *argv[])
{
        // Path to the configuration file.
        std::string help_string = CONFIG_DIRECTORY;    // CONFIG_DIRECTORY - Variable from the CMakeLists.txt in the main project folder.
        help_string.append("/test.config");

        // Read the configuration file and get the parameter.
        SPFR::ReadConfig read_config(help_string);

        std::string read_path = "";
        char character = 'a';
        int read_int = 0;
        float read_float = 0.0;
        bool read_bool_1 = false;
        bool read_bool_2 = false; 
        int read_int_arr[5] = {0, 0, 0, 0, 0};
        float read_float_arr[3] = {0.0, 0.0, 0.0};
        char character_array[7] = {'z', 'z', 'z', 'z', 'z', 'z', 'z'};
        std::string read_str_array[3] = { "", "", "" };

        read_config.get_parameter("string_example", read_path);
        read_config.get_parameter("character", character);
        read_config.get_parameter("number_int", read_int);
        read_config.get_parameter("number_float", read_float);
        read_config.get_parameter("number_bool", read_bool_1);
        read_config.get_parameter("word_bool", read_bool_2);
        read_config.get_parameter("number_int_array", read_int_arr, (int) (sizeof(read_int_arr)/
                                  sizeof(read_int_arr[0])));
        read_config.get_parameter("number_float_array", read_float_arr, (int) (sizeof(read_float_arr)/
                                  sizeof(read_float_arr[0])));
        read_config.get_parameter("character_array", character_array, (int) (sizeof(character_array)/
                                  sizeof(character_array[0])));

        read_config.get_parameter("string_array", read_str_array, (int) (sizeof(read_str_array)/
                                  sizeof(read_str_array[0])));
        
        

	return 0;
}
