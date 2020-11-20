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
 * This library is responsible for reading a configuration file and returns the requested information.
 */
// --------------------------------------------------------------------------------------------------------------------

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <experimental/filesystem>

#include "readConfig.hpp"


namespace SPFR
{

ReadConfig::ReadConfig(std::string config_file_name)
{
    this->_config_file_name = config_file_name;
    this->data_size_tested = false;
    this->data_size_ok = false;
    this->Init();
}

ReadConfig::~ReadConfig()
{

}

void ReadConfig::Init()
{
    std::cout << "File name of the configuration file: " << this->_config_file_name.c_str() << std::endl;

    // Check the size of the configuration file and if it is too big or a directory end the program.
    std::experimental::filesystem::path p = this->_config_file_name.c_str();

    try {
        std::experimental::filesystem::file_size(p);
    } catch(std::experimental::filesystem::filesystem_error& e) {
        std::cout << "Error while reading the configuration file. " << e.what() << '\n';
        return;
    } 

    std::cout << "Size of the configuration file: " << std::experimental::filesystem::file_size(p) << " Bytes" << '\n';

    if(std::experimental::filesystem::file_size(p) > 1048576)
    {
        std::cout << "Size exceeds the limit of 1 MB. Stop reading the file." << '\n';
        return;
    }

    // Read the text file.
    std::ifstream text_config(this->_config_file_name);

    text_config.seekg(0, std::ios::end);

    size_t size = text_config.tellg();

    std::string buffer(size, ' ');

    text_config.seekg(0);

    text_config.read(&buffer[0], size); 

    // Remove all comments from string buffer.
    std::string buffer_new = this->remove_comments(buffer);

    // Seperate the variables from the string.
    this->raw_config = split(buffer_new, ":", " ");

    if (this->raw_config.size()%2 != 0)
    {
        std::cout << std::endl;
        std::cout << "######### ERROR #########" << std::endl;
        std::cout << "The configuration file has one or more wrong parameter." << std::endl;
        std::cout << "Limitations: There are no spaces in the path string allowed. " <<
                     "Comments are also not allowed." << std::endl;

        std::cout << std::endl;
        std::cout << "Here an example of an allowed format:" << std::endl;
        std::cout << std::endl;
        std::cout << "path: /home/USERNAME/workspace/blubb" << std::endl;
        std::cout << "number_int: 12" << std::endl;
        std::cout << "number_float: 4234.234234" << std::endl;
        std::cout << "number_bool_1: 1" << std::endl;
        std::cout << "number_bool_2: false" << std::endl;
        std::cout << "number_array: [2,3,4.5]" << std::endl;
        std::cout << "list: {Hello, World, !}" << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Success in reading the configuration file." << std::endl;
    }
}

std::string ReadConfig::remove_comments(std::string &buffer)
{
    int n = buffer.length(); 
    std::string buffer_new; 
  
    // Flags to indicate that comments have started or not. 
    bool comment = false;
    bool new_line = true;
    bool whole_line_is_comment = false;
  
    for (int i=0; i<n; i++) 
    { 
        // If single line comment flag is on, then check for end of it.
        if (comment == true && buffer[i] == '\n') 
        {
            comment = false;

            // If the comments are at the end of a line with values, keep the "new line".
            if(!whole_line_is_comment)
            {
                buffer_new += '\n'; 
            }
        }   
        // If this character is in a comment, ignore it.
        else if (comment) 
        {
            continue; 
        }
        // Check for beginning of comments and set the approproate flags 
        else if (buffer[i] == '#') 
        {
            comment = true; 

            if (new_line)
            {
                whole_line_is_comment = true;
            }
            else
            {
                whole_line_is_comment = false;
            }
        }
        // If current character is a non-comment character, append it to res 
        else
        {  
            buffer_new += buffer[i]; 
        }

        // Check for new lines.
        if (buffer[i] == '\n')
        {
            new_line = true;
        }
        else
        {
            new_line = false;
        }
    } 

    return buffer_new; 
}

std::vector<std::string> ReadConfig::split(const std::string& s, std::string delimiter, std::string delete_chars)
{
    std::vector<std::string> wordVector;
    std::stringstream stringStream(s);
    std::string line;

    // Split the string according to the "limiter" string and after each new line.
    while(std::getline(stringStream, line)) 
    {
        std::size_t prev = 0, pos;
        while ((pos = line.find_first_of(delimiter, prev)) != std::string::npos)
        {
            if (pos > prev)
            {
                wordVector.push_back(line.substr(prev, pos-prev));
            }

            prev = pos+1;
        }

        if (prev < line.length())
        {
            wordVector.push_back(line.substr(prev, std::string::npos));
        }
    }

    // Remove characters from the sub strings according to the "delete_chars".
    // TODO: Spaces in path strings are NOT supported! I need to fix this.
    for (int j = 0; j < wordVector.size(); j++)
    {
        for (int k = 0; k < delete_chars.size(); k++) 
        {           
            wordVector.at(j).erase(std::remove(wordVector.at(j).begin(), wordVector.at(j).end(), delete_chars[k]),
            wordVector.at(j).end());
        }
    }

    return wordVector;
}

void ReadConfig::check_data_size()
{
    this->data_size_tested = true;

    // If the config file is empty, this gives the user a hint.
    if(this->raw_config.size() == 0)
    {
        std::cout << "Data invalid! Please read the previous output for further information." <<
        " Using initial values..." << std::endl;
    }
    else
    {
        this->data_size_ok = true;
    }
}

void ReadConfig::get_parameter(std::string int_value_name, int &int_value)
{
    int result = -1;

    if(!this->data_size_tested) this->check_data_size();

    if (this->data_size_ok)
    {
        for (int i = 0; i < this->raw_config.size(); i=i+2)
        {
            result = this->raw_config.at(i).compare(int_value_name);

            if (result == 0)
            {
                int_value = std::stoi(this->raw_config.at(i+1));

                std::cout << "Parameter for " << int_value_name.c_str() << " is: " << int_value << std::endl; 

                return;
            }
        }
    }

    if (result != 0)
    {
        std::cout << "Could not find the parameter for " << int_value_name.c_str() << ". Using instead the " <<
        "initial value: " << int_value << std::endl; 
    }

    return;
}


void ReadConfig::get_parameter(std::string char_value_name, char &char_value)
{
    int result = -1;

    if(!this->data_size_tested) this->check_data_size();

    if (this->data_size_ok)
    {
        for (int i = 0; i < this->raw_config.size(); i=i+2)
        {
            result = this->raw_config.at(i).compare(char_value_name);

            if (result == 0)
            {
                if (this->raw_config.at(i+1).length() == 1)
                {
                    std::string str = this->raw_config.at(i+1);
                    std::vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);
                
                    char_value = cstr[0];

                    std::cout << "Parameter for " << char_value_name.c_str() << " is: " << char_value << std::endl; 

                    return;
                }
                else
                {
                    std::cout << "Only one character expected for the parameter " << char_value_name.c_str() << 
                    ", but found more than one. Using instead the initial value: " << char_value << std::endl; 
        
                    return;
                }
            }
        }
    }

    if (result != 0)
    {
        std::cout << "Could not find the parameter for " << char_value_name.c_str() << ". Using instead the " <<
        "initial value: " << char_value << std::endl; 
    }

    return;
}


void ReadConfig::get_parameter(std::string float_value_name, float &float_value)
{
    int result = -1;

    if(!this->data_size_tested) this->check_data_size();

    if (this->data_size_ok)
    {
        for (int i = 0; i < this->raw_config.size(); i=i+2)
        {
            result = this->raw_config.at(i).compare(float_value_name);

            if (result == 0)
            {
                float_value = std::stof(this->raw_config.at(i+1));

                std::cout << "Parameter for " << float_value_name.c_str() << " is: " << float_value << std::endl; 

                return;
            }
        }
    }

    if (result != 0)
    {
        std::cout << "Could not find the parameter for " << float_value_name.c_str() << ". Using instead the " <<
        "initial value: " << float_value << std::endl; 
    }

    return;
}

void ReadConfig::get_parameter(std::string str_value_name, std::string &str_value)
{
    int result = -1;

    if(!this->data_size_tested) this->check_data_size();

    if (this->data_size_ok)
    {
        for (int i = 0; i < this->raw_config.size(); i=i+2)
        {
            result = this->raw_config.at(i).compare(str_value_name);

            if (result == 0)
            {
                str_value = this->raw_config.at(i+1);

                std::cout << "Parameter for " << str_value_name.c_str() << " is: " << str_value << std::endl; 

                return;
            }
        }
    }

    if (result != 0)
    {
        std::cout << "Could not find the parameter for " << str_value_name.c_str() << ". Using instead the " <<
        "initial value: " << str_value << std::endl; 
    }

    return;
}

void ReadConfig::get_parameter(std::string bool_value_name, bool &bool_value)
{
    int result = -1;

    if(!this->data_size_tested) this->check_data_size();

    if (this->data_size_ok)
    {
        for (int i = 0; i < this->raw_config.size(); i=i+2)
        {
            result = this->raw_config.at(i).compare(bool_value_name);

            if (result == 0)
            {
                if (this->raw_config.at(i+1).compare("1") || this->raw_config.at(i+1).compare("true") 
                                                          || this->raw_config.at(i+1).compare("TRUE"))
                {
                    bool_value = true;
                    
                    std::cout << "Parameter for " << bool_value_name.c_str() << " is: true" << std::endl; 

                    return;
                }
                else if (this->raw_config.at(i+1).compare("0") || this->raw_config.at(i+1).compare("false") 
                                                               || this->raw_config.at(i+1).compare("FALSE"))
                {
                    bool_value = false;
                    
                    std::cout << "Parameter for " << bool_value_name.c_str() << " is: false" << std::endl; 

                    return;
                }
                else
                {
                    result = -1;
                    
                    std::cout << "Cannot read the value! Allowed values are: 0, 1, true, false, TRUE and FALSE." << 
                    std::endl; 
                    
                    return;
                }
            }
        }
    }

    if (result != 0)
    {
        std::cout << "Could not find the parameter for " << bool_value_name.c_str() << ". Using instead the " <<
        "initial value: " << bool_value << std::endl; 
    }

    return;
}


void ReadConfig::get_parameter(std::string int_arr_value_name, int *int_arr_value, int int_arr_size)
{    
    std::vector<std::string> temp_str_vector;
    int result = -1;
    
    if(!this->data_size_tested) this->check_data_size();

    if (this->data_size_ok)
    {
        for (int i = 0; i < this->raw_config.size(); i=i+2)
        {
            result = this->raw_config.at(i).compare(int_arr_value_name);

            if (result == 0)
            {
                std::string temp_str;
                
                temp_str = this->raw_config.at(i+1);
            
                // Remove the brackets from the string.
                char delete_chars[] = "()[]{}";
                
                for (int i = 0; i < sizeof(delete_chars)/sizeof(delete_chars[0]); i++)
                {
                    temp_str.erase (std::remove(temp_str.begin(), temp_str.end(), delete_chars[i]), temp_str.end());
                }
                
                // Store all numbers seperated by commata or space in the array fields.
                temp_str_vector = split(temp_str, ",", " ");
                
                if (temp_str_vector.size() != int_arr_size)
                {
                    std::cout << "Error in reading parameter for " << int_arr_value_name.c_str() <<
                    ". Size of the requested array doesn't fit the size of the one in the " <<
                    "configuration file. The size of the array in the executable requested " << int_arr_size <<
                    " fields and the array in the file has " << temp_str_vector.size() << "fields." <<  
                    "Please check the code and file or have a look in the example file." << std::endl;
                    
                    return;
                }
                else
                {
                    for (int i = 0; i < temp_str_vector.size(); i++)
                    {                    
                        int_arr_value[i] = std::stoi(temp_str_vector.at(i));
                    }
                }

                std::cout << "Parameter for " << int_arr_value_name.c_str() << " is: [";

                for (int i = 0; i < int_arr_size; i++)
                {
                    std::cout << int_arr_value[i]; 
            
                    if (i < int_arr_size - 1) std::cout << ", ";
                }
                
                std::cout << "]" << std::endl;

                return;
            }
        }
    }

    if (result != 0)
    {
        std::cout << "Could not find the parameter for " << int_arr_value_name.c_str() << ". Using instead the " <<
        "initial value: [";
        
        for (int i = 0; i < int_arr_size; i++)
        {
            std::cout << int_arr_value[i]; 
            
            if (i < int_arr_size - 1) std::cout << ", ";
        }
        
        std::cout << "]" << std::endl;
    }

    return;
}


void ReadConfig::get_parameter(std::string float_arr_value_name, float *float_arr_value, int float_arr_size)
{    
    std::vector<std::string> temp_str_vector;
    int result = -1;
    
    if(!this->data_size_tested) this->check_data_size();

    if (this->data_size_ok)
    {
        for (int i = 0; i < this->raw_config.size(); i=i+2)
        {
            result = this->raw_config.at(i).compare(float_arr_value_name);

            if (result == 0)
            {
                std::string temp_str;
                
                temp_str = this->raw_config.at(i+1);
            
                // Remove the brackets from the string.
                char delete_chars[] = "()[]{}";
                
                for (int i = 0; i < sizeof(delete_chars)/sizeof(delete_chars[0]); i++)
                {
                    temp_str.erase (std::remove(temp_str.begin(), temp_str.end(), delete_chars[i]), temp_str.end());
                }
                
                // Store all numbers seperated by commata or space in the array fields.
                temp_str_vector = split(temp_str, ",", " ");
                
                if (temp_str_vector.size() != float_arr_size)
                {
                    std::cout << "Error in reading parameter for " << float_arr_value_name.c_str() <<
                    ". Size of the requested array doesn't fit the size of the one in the " <<
                    "configuration file. The size of the array in the executable requested " << float_arr_size <<
                    " fields and the array in the file has " << temp_str_vector.size() << "fields." <<  
                    "Please check the code and file or have a look in the example file." << std::endl;
                    
                    return;
                }
                else
                {
                    for (int i = 0; i < temp_str_vector.size(); i++)
                    {                    
                        float_arr_value[i] = std::stof(temp_str_vector.at(i));
                    }
                }

                std::cout << "Parameter for " << float_arr_value_name.c_str() << " is: [";

                for (int i = 0; i < float_arr_size; i++)
                {
                    std::cout << float_arr_value[i]; 
            
                    if (i < float_arr_size - 1) std::cout << ", ";
                }
                
                std::cout << "]" << std::endl;

                return;
            }
        }
    }

    if (result != 0)
    {
        std::cout << "Could not find the parameter for " << float_arr_value_name.c_str() << ". Using instead the " << 
        "initial value: [";
        
        for (int i = 0; i < float_arr_size; i++)
        {
            std::cout << float_arr_value[i]; 
            
            if (i < float_arr_size - 1) std::cout << ", ";
        }
        
        std::cout << "]" << std::endl;
    }

    return;
}


void ReadConfig::get_parameter(std::string char_arr_value_name, char *char_arr_value, int char_arr_size)
{
    std::vector<std::string> temp_str_vector;
    int result = -1;
    
    if(!this->data_size_tested) this->check_data_size();

    if (this->data_size_ok)
    {
        for (int i = 0; i < this->raw_config.size(); i=i+2)
        {
            result = this->raw_config.at(i).compare(char_arr_value_name);

            if (result == 0)
            {
                std::string temp_str;
                
                temp_str = this->raw_config.at(i+1);
            
                // Remove the brackets from the string.
                char delete_chars[] = "()[]{}";
                
                for (int i = 0; i < sizeof(delete_chars)/sizeof(delete_chars[0]); i++)
                {
                    temp_str.erase (std::remove(temp_str.begin(), temp_str.end(), delete_chars[i]), temp_str.end());
                }
                
                // Store all characters seperated by commata or space in the array fields.
                temp_str_vector = split(temp_str, ",", " ");
                
                if (temp_str_vector.size() != char_arr_size)
                {
                    std::cout << "Error in reading parameter for " << char_arr_value_name.c_str() <<
                    ". Size of the requested array doesn't fit the size of the one in the " << 
                    "configuration file. The size of the array in the executable requested " << char_arr_size <<
                    " fields and the array in the file has " << temp_str_vector.size() << "fields." <<  
                    "Please check the code and file or have a look in the example file." << std::endl;
                    
                    return;
                }
                else
                {   
                    char temp_char[1];
                             
                    for (int i = 0; i < temp_str_vector.size(); i++)
                    {   
                        if (temp_str_vector.at(i).size() == 1)
                        {
                            strcpy(temp_char, temp_str_vector.at(i).c_str());
                            
                            char_arr_value[i] = temp_char[0];
                        }
                        else
                        {
                            result = -1;
                            
                            std::cout << "Error in reading parameter for " << char_arr_value_name.c_str() <<
                            ". Size of field  " << i << " has none or more than one character. Halt..." <<
                            std::endl;
                            
                            return;
                        }
                    } 
                }

                std::cout << "Parameter for " << char_arr_value_name.c_str() << " is: [";

                for (int i = 0; i < char_arr_size; i++)
                {
                    std::cout << char_arr_value[i]; 
            
                    if (i < char_arr_size - 1) std::cout << ", ";
                }
                
                std::cout << "]" << std::endl;

                return;
            }
        }
    }

    if (result != 0)
    {
        std::cout << "Could not find the parameter for " << char_arr_value_name.c_str() << ". Using instead the " << 
        "initial value: [";
        
        for (int i = 0; i < char_arr_size; i++)
        {
            std::cout << char_arr_value[i]; 
            
            if (i < char_arr_size - 1) std::cout << ", ";
        }
        
        std::cout << "]" << std::endl;
    }

    return;
}


void ReadConfig::get_parameter(std::string str_arr_value_name, std::string *str_array_value, int str_arr_size)
{
    std::vector<std::string> temp_str_vector;
    int result = -1;
    
    if(!this->data_size_tested) this->check_data_size();

    if (this->data_size_ok)
    {
        for (int i = 0; i < this->raw_config.size(); i=i+2)
        {
            result = this->raw_config.at(i).compare(str_arr_value_name);

            if (result == 0)
            {
                std::string temp_str;
                
                temp_str = this->raw_config.at(i+1);
            
                // Remove the brackets from the string.
                char delete_chars[] = "()[]{}";
                
                for (int i = 0; i < sizeof(delete_chars)/sizeof(delete_chars[0]); i++)
                {
                    temp_str.erase (std::remove(temp_str.begin(), temp_str.end(), delete_chars[i]), temp_str.end());
                }
                
                // Store all characters seperated by commata or space in the array fields.
                temp_str_vector = split(temp_str, ",", " ");
                
                if (temp_str_vector.size() != str_arr_size)
                {
                    std::cout << "Error in reading parameter for " << str_arr_value_name.c_str() <<
                    ". Size of the requested array doesn't fit the size of the one in the " << 
                    "configuration file. The size of the array in the executable requested " << str_arr_size <<
                    " fields and the array in the file has " << temp_str_vector.size() << "fields." <<  
                    "Please check the code and file or have a look in the example file." << std::endl;
                    
                    return;
                }
                else
                {   
                    for (int i = 0; i < temp_str_vector.size(); i++)
                    {                    
                        str_array_value[i] = temp_str_vector.at(i);
                    }
                }

                std::cout << "Parameter for " << str_arr_value_name.c_str() << " is: [";

                for (int i = 0; i < str_arr_size; i++)
                {
                    std::cout << str_array_value[i]; 
            
                    if (i < str_arr_size - 1) std::cout << ", ";
                }
                
                std::cout << "]" << std::endl;

                return;
            }
        }
    }

    if (result != 0)
    {
        std::cout << "Could not find the parameter for " << str_arr_value_name.c_str() << ". Using instead the " << 
        "initial value: [";
        
        for (int i = 0; i < str_arr_size; i++)
        {
            std::cout << str_array_value[i].c_str(); 
            
            if (i < str_arr_size - 1) std::cout << ", ";
        }
        
        std::cout << "]" << std::endl;
    }

    return;
}


}
