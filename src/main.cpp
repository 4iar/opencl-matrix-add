#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>


std::string readFile(std::string file_name);
std::vector<float> readMatrixFromFile (std::string file_name);

#ifndef DEVICE
#define DEVICE CL_DEVICE_TYPE_DEFAULT
#endif

int main (int argc, char* argv[]) {
    // Handle input arguments
    // ./main <first matrix filename> <second matrix filename> <num columns> <num rows>
    std::string matrix_a_filename(argv[1]);
    std::string matrix_b_filename(argv[2]);
    int num_columns = atoi(argv[3]);
    int num_rows = atoi(argv[4]);

    // Set up the input matrices
    std::vector<float> h_a = readMatrixFromFile(matrix_a_filename);
    std::vector<float> h_b = readMatrixFromFile(matrix_b_filename);

    // and the output (results) matrix
    const unsigned int size = h_a.size();
    std::vector<float> h_results(size);

    try {
        cl::Context context(DEVICE);

        cl::Program program(context, readFile("kernel.cl"), true);

        auto kernel = cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer, int>(program, "d_add_matrices");

        cl::Buffer d_a = cl::Buffer(context, begin(h_a), end(h_a), true);
        cl::Buffer d_b = cl::Buffer(context, begin(h_b), end(h_b), true);
        cl::Buffer d_results = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * size);

        cl::CommandQueue queue(context);
        kernel(cl::EnqueueArgs(queue, cl::NDRange(size)), d_a, d_b, d_results, size);

        queue.finish();

        cl::copy(queue, d_results, h_results.begin(), h_results.end());

        // Unflatten and print the resulting matrix
        for (unsigned int i = 0; i < size; i++) {
            std::cout << h_results[i];
            if ((i + 1) % num_columns == 0) {
                std::cout << std::endl;
            } else {
                std::cout << ',';
            }
        };
        std::cout << std::endl;

    } catch (cl::Error err) {
        std::cout << "Exception\n";
        std::cerr
                << "ERROR: "
                << err.what()
                << "("
                << err.err()
                << ")"
                << std::endl;
    }
}

/**
 * \brief Read a matrix from a file and flatten it
 *
 */
std::vector<float> readMatrixFromFile (std::string file_name) {
    // Read the matrix into a string, which has the format:
    // n1,n2,n3,n4
    // n5,n6,n7,n8
    // n9,...
    // (where line breaks are \n)
    std::string raw_matrix;
    std::ifstream file(file_name);

    std::string line;
    while (std::getline(file, line)) {
        raw_matrix += line;
        // Add a comma so that we can more easily flatten the matrix
        // and handle \n
        raw_matrix += ",";
    }
    // Remove the last trailing comma
    raw_matrix.pop_back();

    // Flatten the matrix by splitting the strings using the , separator
    // {n1,n2,n3,n4,n5,n6,n7,n8,n9...}
    std::vector<std::string> flattened_matrix;
    boost::split(flattened_matrix, raw_matrix, boost::is_any_of(","));

    // Finally we convert the elements to floats
    std::vector<float> matrix;
    std::transform(flattened_matrix.begin(),
                   flattened_matrix.end(),
                   std::back_inserter(matrix),
                   [](std::string element) {
                       return std::stof(element);
                   });

    return matrix;
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
