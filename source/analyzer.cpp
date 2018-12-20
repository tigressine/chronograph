// A driver program that analyzes the parallel-divsufsort library. This isn't
// the cleanest C++ you'll ever see. Please forgive my inevitable blasphemies.

// Written by Tiger Sachse.

#include <cmath>
#include <string>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream> // for ostringstream
#include <vector>
#include <divsufsort.h>
#include <tclap/CmdLine.h>

#ifndef PAROPENMP
#ifndef PARCILKPLUS
#define PARCILKPLUS 1
#endif
#endif

#if defined(PARCILKPLUS) // cilk++
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#elif defined(PAROPENMP) // openmp
#include <omp.h>
#else // c++
#endif

void set_num_threads(int nthreads){

#if defined(PARCILKPLUS)
  std::ostringstream sbuf;
  sbuf << nthreads ;
  __cilkrts_end_cilk();
  __cilkrts_set_param("nworkers", sbuf.str().c_str());
#elif defined(PAROPENMP)
   omp_set_dynamic(0);
   omp_set_num_threads(nthreads);
#else

#endif

}


// Run timed divsufsort tests for a string using a range of thread counts.
std::vector<double>
run_tests(const std::string& text, int max_threads, int max_runs) {
    auto size = text.size();
    auto suffix_array = std::vector<int64_t>(size);
    auto total_times = std::vector<std::chrono::milliseconds>(max_threads);

    // For each run, test the function using a range of thread caps.
    for (int thread = 2; thread <= max_threads; thread++) {

        // Set the number of threads for this function call.
        set_num_threads(thread);

        for (int run = 0; run < max_runs; run++) {

            // Run the function and save the start and end times.
            auto start = std::chrono::steady_clock::now();
            divsufsort((sauchar_t*) text.data(), suffix_array.data(), size);
            auto end = std::chrono::steady_clock::now();

            // Save the time difference in the total_times array. The
            // average across all runs will be calculated from this sum later.
            auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            total_times[thread - 1] += difference;
        }
    }

    // Calculate the averages for every thread count.
    auto averages = std::vector<double>(max_threads);
    for (int thread = 1; thread <= max_threads; thread++) {
        averages[thread - 1] = total_times[thread - 1].count() / 1000.0 / max_runs;
    }

    return averages;
}

// Write the averages array to a data output file.
void write_averages(std::vector<double>& averages, int max_threads, char* input_name) {
    if (averages.size() == 0 || input_name == NULL) {
        std::cout << "Passed NULL to write_averages!\n";

        exit(1);
    }

    // Create the output file name string.
    std::string input_string(input_name);
    auto start = input_string.find_last_of("/");
    auto stop = input_string.find_last_of(".");
    start = (start == std::string::npos) ? 0 : start + 1;
    stop = (stop == std::string::npos) ? input_string.length() : stop - 1;
    std::string output_name = input_string.substr(start, stop - start + 1) + ".out";

    // Open the output file.
    std::ofstream output_file(output_name);
    if (output_file.fail()) {
        std::cout << "File '" << output_name << "' could not be written.\n";

        exit(1);
    }

    // Write the results to the output file.
    output_file << "Results for '" << input_name << "'\n";
    for (int thread = 1; thread <= max_threads; thread++) {
        output_file << "Thread count: " << thread;
        output_file << " | Seconds: " << averages[thread - 1] << "\n";
    }

    output_file.close();
}

int parse_args(int argc, char *argv[], std::string& fname, int& maxt, int& nruns){
    try{
        TCLAP::CmdLine cmd("Evaluate performance of parallel-divsufsort", ' ', "0.1");
        TCLAP::UnlabeledValueArg<std::string>  infile( "infile", "Input file", true,
                                                       "",
                                                       "FILE"  );
        cmd.add( infile );
        TCLAP::UnlabeledValueArg<int>  max_threads( "max_threads", "Max. Threads", true,
                                                   4,
                                                  "INTEGER"  );
        cmd.add( max_threads );
        TCLAP::UnlabeledValueArg<int>  num_runs( "num_runs", "Max. Runs", true, 1,
                                                "INTEGER"  );
        cmd.add( num_runs );

        cmd.parse(argc, argv);

        fname = infile.getValue();
        maxt = max_threads.getValue();
        nruns = num_runs.getValue();


    } catch (TCLAP::ArgException& ex){
        std::cerr << "Arg Error : " << ex.error() 
                  << " for " << ex.argId() << std::endl;
        return -1;
    }
    return 0;
}

// Main function and entry point of the program.
int main(int argument_count, char* arguments[]) {
    // Parse max_threads and max_runs from the input vector.
    std::string infile;
    int max_threads;
    int max_runs;

    // If the user has not entered enough arguments, scream about it.
    if (parse_args(argument_count, arguments,
                   infile, max_threads, max_runs) != 0) {
        return -1;
    }

    // Attempt to open the text file.
    std::ifstream input(infile.c_str());
    if (input.fail()) {
        std::cout << "File '" << infile << "' could not be read.\n";
        return -1;
    }


    // Create a string from the text file.
    std::string text;
    input.seekg(0, std::ios::end);   
    text.reserve(input.tellg());
    input.seekg(0, std::ios::beg);
    text.assign((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    // Capture averages for different thread counts from the tests.
    // Write these averages to a data file, then free the averages array.
    auto averages = run_tests(text, max_threads, max_runs);
    write_averages(averages, max_threads, arguments[1]);

    return 0;
}
