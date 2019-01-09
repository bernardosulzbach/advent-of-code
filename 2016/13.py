import advent
import collections

GRID_SIDE = 200


def evaluate(x, y, number):
    return bin((x * x + 3 * x + 2 * x * y + y + y * y) + number).count("1") % 2 == 0


def find_path_cost(grid, source, destination):
    seen = set()
    queue = collections.deque()
    queue.append((source, 0))
    while queue:
        element = queue.popleft()
        seen.add(element[0])
        x = element[0][0]
        y = element[0][1]
        cost = element[1]
        if (x, y) == destination:
            return cost
        options = [((x - 1, y), cost + 1), ((x, y - 1), cost + 1), ((x, y + 1), cost + 1), ((x + 1, y), cost + 1)]
        for option in options:
            option_x = option[0][0]
            option_y = option[0][1]
            if option_y < 0 or option_y >= len(grid):
                continue
            if option_x < 0 or option_x >= len(grid[option_y]):
                continue
            if option[0] not in seen and grid[option_y][option_x]:
                queue.append(option)
                seen.add(option[0])
    return None


def make_grid(number, width, height):
    grid = []
    for y in range(height):
        grid.append([])
        for x in range(width):
            grid[y].append(evaluate(x, y, number))
    return grid


def part_one():
    number = int(advent.read_lines(advent.derive_filename(13))[0])
    grid = make_grid(number, GRID_SIDE, GRID_SIDE)
    print(find_path_cost(grid, (1, 1), (7, 4) if advent.is_using_sample() else (31, 39)))


def count_reachable(grid, source, distance):
    seen = set()
    queue = collections.deque()
    queue.append((source, 0))
    while queue:
        element = queue.popleft()
        seen.add(element[0])
        x = element[0][0]
        y = element[0][1]
        cost = element[1]
        if cost == distance:
            continue
        options = [((x - 1, y), cost + 1), ((x, y - 1), cost + 1), ((x, y + 1), cost + 1), ((x + 1, y), cost + 1)]
        for option in options:
            option_x = option[0][0]
            option_y = option[0][1]
            if option_y < 0 or option_y >= len(grid):
                continue
            if option_x < 0 or option_x >= len(grid[option_y]):
                continue
            if option[0] not in seen and grid[option_y][option_x]:
                queue.append(option)
                seen.add(option[0])
    return len(seen)


def part_two():
    number = int(advent.read_lines(advent.derive_filename(13))[0])
    grid = make_grid(number, GRID_SIDE, GRID_SIDE)
    print(count_reachable(grid, (1, 1), 50))


if __name__ == '__main__':
    part_one()
    part_two()
