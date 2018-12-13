"""Analyze parallelized divsufsort data.

Written by Tiger Sachse.
"""
import re
import sys
from pathlib import Path
import matplotlib.pyplot as pyplot

LINE_FORMAT = (
    r'^Thread count: (?P<count>[0-9]+) \| '
    + r'Seconds: (?P<seconds>[0-9]+(\.[0-9]+)?)\n$'
)

def percent_change(num1, num2):
    return (num1 - num2) / abs(num1) * 100

try:
    counts = []
    seconds = []

    # Open the input data file.
    with open(sys.argv[1], 'r') as file_pointer:
        
        # Skip the header line.
        file_pointer.readline()

        # For each line in the file, extract the count and seconds, then
        # add each to its respective list.
        for line in file_pointer:
            line_match = re.match(LINE_FORMAT, line)
            if line_match:
                counts.append(int(line_match.group('count')))
                seconds.append(float(line_match.group('seconds')))
            else:
                raise SyntaxError
except IndexError:
    print('Please provide a data file as your first argument to this script.')
    exit(0)
except SyntaxError:
    print('The data file\'s syntax is incorrect.')
    exit(0)
except FileNotFoundError:
    print('The data file provided could not be found/opened.')
    exit(0)

# Plot the counts and seconds using matplotlib.
pyplot.plot(counts, seconds)
pyplot.xlabel('Thread count')
pyplot.ylabel('Seconds')

# Save the graph as a PNG.
pyplot.savefig(str(Path(sys.argv[1]).stem) + '_seconds_graph.png')
pyplot.clf()

changes = []
for position, amount in enumerate(seconds[:-1]):
    changes.append(percent_change(amount, seconds[position + 1]))

pyplot.plot(counts[1:], changes)
pyplot.xlabel('Thread count')
pyplot.ylabel('Percent change in execution time')

pyplot.savefig(str(Path(sys.argv[1]).stem) + '_changes_graph.png')
