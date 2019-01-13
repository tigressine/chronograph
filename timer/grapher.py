"""Graph parallelized divsufsort timing data.

Written by Tiger Sachse.
"""
import re
import sys
from pathlib import Path
import matplotlib.pyplot as pyplot
from matplotlib.ticker import MaxNLocator

OUTPUT_FILE_FORMAT = '{0}.png'
LINE_FORMAT = (
    r'^Threads: (?P<thread_count>[0-9]+) \| '
    + r'Sort Type B\*: (?P<bstar_time>[0-9]+(\.[0-9]+)?)s \| '
    + r'Construct SA: (?P<construct_sa_time>[0-9]+(\.[0-9]+)?)s \| '
    + r'Divsufsort: (?P<divsufsort_time>[0-9]+(\.[0-9]+)?)s\n$'
)

# Print a panic message and exit the program.
def panic(message):
    print(message)
    exit(1)

try:
    bstar_times = []
    thread_counts = []
    divsufsort_times = []
    construct_sa_times = []

    # Open the input data file.
    with open(sys.argv[1], 'r') as f:
        
        # Skip the header line.
        f.readline()

        # For each line in the file, extract the thread count and all of the
        # appropriate timings, then add each to its respective list.
        for line in f:
            line_match = re.match(LINE_FORMAT, line)
            if line_match:
                bstar_times.append(float(line_match.group('bstar_time')))
                thread_counts.append(int(line_match.group('thread_count')))
                divsufsort_times.append(float(line_match.group('divsufsort_time')))
                construct_sa_times.append(float(line_match.group('construct_sa_time')))
            else:
                raise SyntaxError
except IndexError:
    panic('Please provide a data file as your first argument to this script.')
except SyntaxError:
    panic('The data file\'s syntax is incorrect.')
except FileNotFoundError:
    panic('The data file provided could not be found/opened.')
except Exception:
    panic('Something went wrong!')

# Get the name of the input file.
file_stem = str(Path(sys.argv[1]).stem)

# Set the x axis to only ever be whole integers.
axis = pyplot.figure().gca()
axis.xaxis.set_major_locator(MaxNLocator(integer=True))

# Plot the overall function timing using matplotlib.
pyplot.plot(thread_counts, divsufsort_times)
pyplot.plot(thread_counts, bstar_times)
pyplot.plot(thread_counts, construct_sa_times)
pyplot.title('DivSufSort execution time for \'{0}\''.format(file_stem.upper()))
pyplot.xlabel('Thread count')
pyplot.ylabel('Seconds')
pyplot.legend(['DivSufSort', 'Sort B*', 'Construct SA'], loc='upper center')

# Save the graph.
pyplot.savefig(OUTPUT_FILE_FORMAT.format(file_stem))
