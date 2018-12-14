# chronograph
This project times and analyzes the `divsufsort()` function in the [parallel-divsufsort](https://github.com/jlabeit/parallel-divsufsort) library. It runs the function with a range of thread counts, and then creates a graph of the results using `matplotlib`.

# download
To download and configure this project, execute the following commands. Note that you will need `bash` and `git` installed to start.
```
git clone https://github.com/tgsachse/chronograph.git
cd chronograph
sudo bash chronograph.sh --install-deps
```

# usage
This project analyzes the `divsufsort()` function and graphs the results separately. To analyze the function and produce a results data file, execute the following command:
```
bash chronograph.sh --analyze <file> <max_threads> <max_runs>
```
- `<file>` : The path to the text file you'd like to analyze.
- `<max_threads>` : The (integer) maximum thread count you'd like to analyze the function over. The function will be tested for \[1, `max_threads`\] threads.
- `<max_runs>` : The (integer) maximum number of times the function should be called for each thread count. The timings of these calls will be averaged out, so a higher `max_runs` value will increase accuracy but will also take longer to execute.   

To create a sleek line graph with your new data, execute the following command:
```
bash chronograph.sh --graph <file>
```
- `<file>` : The path to the data file you'd like to graph.

# example
The following example takes the input file `my_stuff.txt`, and tests `divsufsort()` on a range of \[1, 32\] threads. For each thread count, the function is called 10 times. The final results are saved in a data file named `my_stuff.out`:
```
bash chronograph.sh --analyze my_stuff.txt 32 10
```

Next, we graph the resulting data file like so:
```
bash chronograph.sh --graph my_stuff.out
```
This produces a new graph image named `my_stuff_graph.png`.
