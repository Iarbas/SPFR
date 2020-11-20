// ========================================== max. 120 symbols in one line ============================================
/**
 * @par Project
 * SPFR - Simple Parameter File Reader
 *
 * @file    readConfig.hpp
 * @author  Andre Alexander Pieper
 * @version 1.0
 * @date    2020-11-03
 *
 * @brief   Header file to read the configuration file.
 *
 * This library is responsible for reading a configuration file and returns the requested information.
 */
// --------------------------------------------------------------------------------------------------------------------

#ifndef HEADER_READER_HPP_AP_03112020
#define HEADER_READER_HPP_AP_03112020

#include <string>
#include <vector>
#include <list>


namespace SPFR
{
/** @class ReadConfig
 *  @brief Header file to read the configuration file.
 *
 *  This program part is responsible for reading the configuration file and returns
 *  the requested information.
 */
class ReadConfig
{
public:
	ReadConfig(std::string config_file_name);
	virtual ~ReadConfig();				// Virtual Destructor.
        void get_parameter(std::string str_value_name, std::string &str_value);
        void get_parameter(std::string char_value_name, char &char_value);
        void get_parameter(std::string int_value_name, int &int_value);
        void get_parameter(std::string float_value_name, float &float_value);
        void get_parameter(std::string bool_value_name, bool &bool_value);
        void get_parameter(std::string int_arr_value_name, int *int_arr_value, int int_arr_size);
        void get_parameter(std::string float_arr_value_name, float *float_arr_value, int float_arr_size);
        void get_parameter(std::string char_arr_value_name, char *char_arr_value, int char_arr_size);
        void get_parameter(std::string str_arr_value_name, std::string *str_array_value, int str_arr_size);

private:
	void Init();
        void check_data_size();
        bool data_size_tested;
        bool data_size_ok;
	std::string remove_comments(std::string &buffer);
	std::string _config_file_name;
        std::vector<std::string> split(const std::string& s, std::string delimiter, std::string delete_chars);
        std::vector<std::string> raw_config;
};
}

#endif // HEADER_READER_HPP_AP_03112020
