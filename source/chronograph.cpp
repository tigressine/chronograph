// Driver program to test the parallel-divsufsort library.
// Written by Tiger Sachse.

#include <cmath>
#include <string>
#include <chrono>
#include <fstream>
#include <iostream>
#include <divsufsort.h>
#include <cilk/cilk_api.h>

// Run timed divsufsort tests for a string using a range of thread counts.
void run_tests(const std::string& text, int max_threads, int max_runs) {
    auto size = text.size();
    auto suffix_array = new int64_t[size];
    auto total_times = new std::chrono::milliseconds[max_threads]();

    // For each run, test the function using a range of thread caps.
    for (int run = 0; run < max_runs; run++) {
        for (int thread = 1; thread <= max_threads; thread++) {
            char* buffer = new char[(int) std::log10(max_threads) + 2]();
            sprintf(buffer, "%d", thread);
            __cilkrts_end_cilk();
            __cilkrts_set_param("nworkers", buffer);
            delete[] buffer;

            auto start = std::chrono::steady_clock::now();
            divsufsort((sauchar_t*) text.data(), suffix_array, size);
            auto end = std::chrono::steady_clock::now();

            // Save the time difference in the total_times array. The
            // average across all runs will be calculated from this sum later.
            auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            total_times[thread - 1] += difference;
        }
    }

    // Print the average run time for each thread cap.
    for (int thread = 1; thread <= max_threads; thread++) {
        printf("%d thread%s: ", thread, (thread == 1) ? "" : "s");
        printf("%lf seconds\n", total_times[thread - 1].count() / 1000.0 / max_runs);
    }

    // Clean up after yourself!
    delete[] total_times;
    delete[] suffix_array;
}

// Main function and entry point of the program.
int main(int argument_count, char* arguments[]) {

    // If the user has not entered enough arguments, scream about it.
    if (argument_count < 4) {
        std::cout << "Usage: ./chronograph [file] [max threads] [max runs]\n";

        return -1;
    }

    // Attempt to open the text file.
    std::ifstream input(arguments[1]);
    if (input.fail()) {
        std::cout << "File '" << arguments[1] << "' could not be read.\n";

        return -1;
    }

    // Parse max_threads and max_runs from the input vector.
    int max_threads = std::stoi(arguments[2], NULL, 10);
    int max_runs = std::stoi(arguments[3], NULL, 10);

    // Create a string from the text file.
    std::string text;
    input.seekg(0, std::ios::end);   
    text.reserve(input.tellg());
    input.seekg(0, std::ios::beg);
    text.assign((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    run_tests(text, max_threads, max_runs);

    return 0;
}
