import sys
import os
import collections

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


class Computer:
    def __init__(self, instructions):
        self.instruction_pointer = 0
        self.instructions = instructions
        self.registers = [0] * 4

    def get_value(self, value):
        if value.isalpha():
            return self.registers[self.get_register_index(value)]
        return int(value)

    @staticmethod
    def get_register_index(value):
        return ord(value) - ord('a')

    def step(self):
        instruction = self.instructions[self.instruction_pointer]
        if instruction[0] == 'cpy':
            self.registers[self.get_register_index(instruction[2])] = self.get_value(instruction[1])
        elif instruction[0] == 'inc':
            self.registers[self.get_register_index(instruction[1])] += 1
        elif instruction[0] == 'dec':
            self.registers[self.get_register_index(instruction[1])] -= 1
        elif instruction[0] == 'jnz':
            if self.get_value(instruction[1]):
                self.instruction_pointer += int(instruction[2]) - 1
        self.instruction_pointer += 1

    def execute(self):
        while 0 <= self.instruction_pointer < len(self.instructions):
            self.step()


def print_grid(grid):
    for row in grid:
        for cell in row:
            print(int(cell), end='')
        print()


def find_grid_distance(grid, a, b):
    seen = set()
    queue = collections.deque()
    queue.append((a, 0))
    while queue:
        node, cost = queue.popleft()
        if node == b:
            return cost
        if node in seen:
            continue
        seen.add(node)
        i, j = node
        for next_node in ((i - 1, j), (i, j - 1), (i, j + 1), (i + 1, j)):
            next_node_i, next_node_j = next_node
            if next_node_i < 0 or next_node_i >= len(grid):
                continue
            if next_node_j < 0 or next_node_j >= len(grid[next_node_i]):
                continue
            if grid[next_node_i][next_node_j]:
                queue.append((next_node, cost + 1))
    return None


def find_minimum_time_to_visit_all(nodes, distances, starting_point, returning):
    queue = collections.deque()
    cheapest = None
    queue.append(({starting_point}, starting_point, 0))
    while queue:
        seen, last, cost = queue.popleft()
        if len(seen) == len(nodes):
            total_cost = cost
            if returning:
                total_cost += distances[(last, starting_point)]
            if cheapest is None or cheapest > total_cost:
                cheapest = total_cost
            continue
        for node in nodes:
            if node not in seen:
                new_seen = set(seen)
                new_seen.add(node)
                new_last = node
                new_cost = cost + distances[(last, node)]
                queue.append((new_seen, new_last, new_cost))
    return cheapest
