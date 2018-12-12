#include <string>
#include <chrono>
#include <fstream>
#include <iostream>
#include <divsufsort.h>

// Run timed divsufsort tests for a string using a range of thread counts.
void run_tests(const std::string& text, int max_threads, int max_runs) {
    int64_t size = text.size();
    int64_t *suffix_array = new int64_t[size];

    for (int run = 0; run < max_runs; run++) {
        auto start = std::chrono::steady_clock::now();
        divsufsort((sauchar_t*) text.data(), suffix_array, size);
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;

        auto seconds = std::chrono::duration <double, std::milli> (diff).count() / 1000.0;
        std::cout << seconds << "\n";
    }

    delete[] suffix_array;
}

// Main function and entry point of the program.
int main(int argument_count, char* arguments[]) {

    // If the user has not entered enough arguments, scream about it.
    if (argument_count < 3) {
        std::cout << "Usage: ./chronograph [max threads] [file]\n";

        return -1;
    }

    // Parse the max_threads argument from the input vector.
    int max_threads = std::stoi(arguments[1], NULL, 10);

    // Attempt to open the text file.
    std::ifstream input(arguments[2]);
    if (input.fail()) {
        std::cout << "File '" << arguments[2] << "' could not be read.\n";

        return -1;
    }

    // Create a string from the text file.
    std::string text;
    input.seekg(0, std::ios::end);   
    text.reserve(input.tellg());
    input.seekg(0, std::ios::beg);
    text.assign((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    run_tests(text, max_threads, 10);

    return 0;
}
