"""Graph parallelized divsufsort timing data.

Written by Tiger Sachse.
"""
import re
import sys
from pathlib import Path
import matplotlib.pyplot as pyplot

OUTPUT_FILE_FORMAT = '{0}_graph.png'
LINE_FORMAT = (
    r'^Thread count: (?P<count>[0-9]+) \| '
    + r'Seconds: (?P<seconds>[0-9]+(\.[0-9]+)?)\n$'
)

# Print a panic message and exit the program.
def panic(message):
    print(message)
    exit(1)

try:
    counts = []
    seconds = []

    # Open the input data file.
    with open(sys.argv[1], 'r') as f:
        
        # Skip the header line.
        f.readline()

        # For each line in the file, extract the count and seconds, then
        # add each to its respective list.
        for line in f:
            line_match = re.match(LINE_FORMAT, line)
            if line_match:
                counts.append(int(line_match.group('count')))
                seconds.append(float(line_match.group('seconds')))
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

# Plot the counts and seconds using matplotlib.
pyplot.plot(counts, seconds)
pyplot.xlabel('Thread count')
pyplot.ylabel('Seconds')

# Save the graph.
pyplot.savefig(OUTPUT_FILE_FORMAT.format(str(Path(sys.argv[1]).stem)))
