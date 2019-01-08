import sys
import os

SAMPLE_FLAG = '-s'

SAMPLE_PATH = 'input/sample/'
PUZZLE_PATH = 'input/puzzle/'

EXTENSION = 'txt'


def is_using_sample():
    return SAMPLE_FLAG in sys.argv


def derive_filename(problem_number: int):
    path = SAMPLE_PATH if is_using_sample() else PUZZLE_PATH
    return os.path.join(path, '{:02}.{}'.format(problem_number, EXTENSION))

def read_lines(filename: str):
    lines = []
    with open(filename) as file_handle:
        for line in file_handle.readlines():
            if line and line[-1] == '\n':
                line = line[:-1]
            lines.append(line)
    return lines
