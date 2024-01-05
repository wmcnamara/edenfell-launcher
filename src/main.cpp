#include <iostream>
#include <regex>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>

#ifndef _WIN32
static_assert(false, "Windows is required to compile this program, it does not support other platforms.");
#endif

#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


std::string to_lower(std::string str) 
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });

    return str;
}

void get_ip_and_port_from_user(std::string& ip_address_out, std::string& port_out, const std::regex& ip_address_regex, const std::regex& port_regex)
{
    bool has_good_ip_address = false;
    while (!has_good_ip_address)
    {
        std::cout << "Enter an IP Address to connect or host on: ";
        getline(std::cin, ip_address_out);

        if (!std::regex_match(ip_address_out, ip_address_regex))
        {
            std::cout << "IP Address is invalid\n";
            continue;
        }
      
        has_good_ip_address = true;
    }

    bool has_good_port_number = false;
    while (!has_good_port_number) 
    {
        std::cout << "Enter an port number to connect or host on: ";
        getline(std::cin, port_out);

        if (!std::regex_match(port_out, port_regex))
        {
            std::cout << "Port number is invalid\n";
            continue;
        }

        has_good_port_number = true;
    }

    std::cout << "\n" << "Connection data: " << ip_address_out << ":" << port_out << '\n';
}

void get_connection_type_from_user(std::string& connection_type_out) 
{
    bool has_good_connection_type = false;
    while (!has_good_connection_type)
    {
        std::cout << "Type \"host\", \"server\", or \"client\" to choose how you will connect: \n";
        getline(std::cin, connection_type_out);

        connection_type_out = to_lower(connection_type_out);

        if (connection_type_out != "host" && connection_type_out != "client" && connection_type_out != "server")
        {
            std::cout << "invalid connection type\n";
        }
        else
        {
            has_good_connection_type = true;
        }
    }
}


int main(const char** argv) 
{
    const std::string edenfell_executable_path = "edenfell.exe";

//Disable regex escape code warning
#pragma warning( disable : 4129 )
#pragma warning( push )
    const std::regex ip_address_regex("[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}");
#pragma warning( disable : 4129 )

    const std::regex port_regex("^([1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$");

    //Get connection type (host, server or client)
    std::string connection_type;
    get_connection_type_from_user(connection_type);

    //Get the IP address and port number
    std::string ip_address, port_number;
    get_ip_and_port_from_user(ip_address, port_number, ip_address_regex, port_regex);

    //Create CLI data
    std::string command_line_arguments; 
    command_line_arguments += connection_type;
    command_line_arguments += " " + ip_address;
    command_line_arguments += " " + port_number;

    //Copy the command line arguments into a vector to get a non const char pointer to pass to CreateProcessA
    std::vector<char> cla_character_buffer(command_line_arguments.begin(), command_line_arguments.end());

    //Create a full filepath for Edenfell to output
    std::filesystem::path full_executable_path = std::filesystem::current_path();
    full_executable_path /= std::filesystem::path(edenfell_executable_path);

    // Start the edenfell process
    STARTUPINFO startup_info{};
    PROCESS_INFORMATION process_information{};

    std::cout << "Trying to start Edenfell process: " << full_executable_path.string() << ' ' << command_line_arguments << '\n';

    bool succeeded = CreateProcessA
    (
        edenfell_executable_path.c_str(),     // Executable directory
        cla_character_buffer.data(),          // Command line
        NULL,                                 // Process handle not inheritable
        NULL,                                 // Thread handle not inheritable
        FALSE,                                // Set handle inheritance to FALSE
        0,                                    // No creation flags
        NULL,                                 // Use parent's environment block
        NULL,                                 // Use parent's starting directory 
        &startup_info,                        // Pointer to STARTUPINFO structure
        &process_information                  // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );

    if (succeeded) 
    {
        std::cout << "Edenfell process successfully launched\n";
    }
    else 
    {
        std::cout << "Edenfell process failed to launch. Please check that the directory for Edenfell is correct.\n";

        //Wait until the user presses enter to close
        std::string wait_str;
        std::getline(std::cin, wait_str);
    }

    //If it succeeds, wait until the process dies to kill the launcher
    WaitForSingleObject(process_information.hProcess, 10000);

    CloseHandle(process_information.hProcess);
    CloseHandle(process_information.hThread);
}