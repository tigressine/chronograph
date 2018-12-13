import re
import sys
import matplotlib.pyplot as pyplot
from pathlib import Path

LINE_FORMAT = (
    r'^Thread count: (?P<count>[0-9]+) \| '
    + r'Seconds: (?P<seconds>[0-9]+(\.[0-9]+)?)\n$'
)

try:
    counts = []
    seconds = []
    with open(sys.argv[1], 'r') as file_pointer:
        
        # Skip the header line.
        file_pointer.readline()

        # For each line in the file, extract the count and seconds.
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

pyplot.plot(counts, seconds)
pyplot.xlabel('Thread count')
pyplot.ylabel('Seconds')

pyplot.savefig(str(Path(sys.argv[1]).stem) + '_seconds_graph.png')
