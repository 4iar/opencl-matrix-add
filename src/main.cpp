#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <CL/cl.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>


std::string readFile(std::string file_name);
std::vector<float> readMatrixFromFile (std::string file_name, double columns, double rows);

#ifndef DEVICE
#define DEVICE CL_DEVICE_TYPE_DEFAULT
#endif

int main (int argc, char* argv[]) {

}

/**
 * \brief Read a matrix from a file
 *
 */
std::vector<float> readMatrixFromFile (std::string file_name, double columns, double rows) {
}

/**
 * \brief Read a file into a string
 *
 */
std::string readFile(std::string file_name) {
    std::ifstream stream(file_name.c_str());
    if (!stream.is_open()) {
        throw std::invalid_argument("Cannot open file: " + file_name);
    }

    return std::string(
            std::istreambuf_iterator<char>(stream),
            (std::istreambuf_iterator<char>()));
}
