import advent
import collections


class Cell:
    def __init__(self, is_goal, size, used, available):
        self.is_goal = is_goal
        self.size = size
        self.used = used
        self.available = available

    def to_string(self, available_in_empty_cell):
        if self.is_goal:
            return 'G'
        elif self.used > available_in_empty_cell:
            return '#'
        elif self.used == 0:
            return '_'
        else:
            return '.'


def make_grid():
    grid = []

    def set_cell(x, y, values):
        while len(grid) <= y:
            grid.append([])
        while len(grid[y]) <= x:
            grid[y].append(None)
        grid[y][x] = values

    cells = advent.read_lines(advent.derive_filename(22))[1:]
    for cell in cells:
        split = cell.split()
        cell_x = int(split[0].split('-')[1][1:])
        cell_y = int(split[0].split('-')[2][1:])
        set_cell(cell_x, cell_y, Cell(False, int(split[1][:-1]), int(split[2][:-1]), int(split[3][:-1])))
    grid[0][-1].is_goal = True
    return grid


def part_one():
    values = []
    for instruction in advent.read_lines(advent.derive_filename(22))[1:]:
        split = instruction.split()
        values.append([split[0], int(split[1][:-1]), int(split[2][:-1]), int(split[3][:-1])])
    count = 0
    for a in values:
        for b in values:
            if 0 < a[2] <= b[3] and a[0] != b[0]:
                count += 1
    print(count)


def find_path(grid, x_a, y_a, x_b, y_b):
    available = grid[y_a][x_a].available
    seen = set()
    queue = collections.deque()
    queue.append([(x_a, y_a)])
    destination = (x_b, y_b)
    while queue:
        alternative = queue.popleft()
        seen.add(alternative[-1])
        x = alternative[-1][0]
        y = alternative[-1][1]
        if alternative[-1] == destination:
            return alternative
        options = [(x - 1, y), (x, y - 1), (x, y + 1), (x + 1, y)]
        for option in options:
            option_x = option[0]
            option_y = option[1]
            if option_y < 0 or option_y >= len(grid):
                continue
            if option_x < 0 or option_x >= len(grid[option_y]):
                continue
            option_cell = grid[option_y][option_x]
            if option not in seen and (not option_cell.is_goal) and option_cell.used <= available:
                seen.add(option)
                complete_option_path = alternative.copy()
                complete_option_path.append(option)
                queue.append(complete_option_path)
    return None


def print_grid(grid):
    available_in_empty_cell = 0
    for row in grid:
        for cell in row:
            if cell.used == 0:
                available_in_empty_cell = max(available_in_empty_cell, cell.available)
            if cell.is_goal:
                goal_cell_size = cell.size
    for row in grid:
        for cell in row:
            print(cell.to_string(available_in_empty_cell), end='')
        print()


def part_two():
    moves = 0
    grid = make_grid()

    def swap(x_a, y_a, x_b, y_b):
        if (x_a, y_a) == (x_b, y_b):
            return
        nonlocal moves
        moves += 1
        grid[y_a][x_a], grid[y_b][x_b] = grid[y_b][x_b], grid[y_a][x_a]

    goal_x = len(grid[0]) - 1

    while goal_x != 0:
        def reposition_empty():
            empty_x = 0
            empty_y = 0
            for y, row in enumerate(grid):
                for x, cell in enumerate(row):
                    if cell.used == 0:
                        empty_x = x
                        empty_y = y
            path = find_path(grid, empty_x, empty_y, goal_x - 1, 0)
            for (x, y) in path:
                swap(x, y, empty_x, empty_y)
                empty_x = x
                empty_y = y

        reposition_empty()
        swap(goal_x - 1, 0, goal_x, 0)
        goal_x -= 1
    print(moves)


if __name__ == '__main__':
    part_one()
    part_two()
