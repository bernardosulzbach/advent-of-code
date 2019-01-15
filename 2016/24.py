import advent

if __name__ == '__main__':
    maze = []
    for line in advent.read_lines(advent.derive_filename(24))[1:]:
        maze.append(list(line))
    digits = set()
    for row in maze:
        for cell in row:
            if cell.isdigit():
                digits.add(cell)
    grid = []
    for row in maze:
        grid.append([cell != '#' for cell in row])
    distances = {}
    for digit_a in digits:
        for digit_b in digits:
            a = None
            b = None
            for i, row in enumerate(maze):
                for j, cell in enumerate(row):
                    if maze[i][j] == digit_a:
                        a = (i, j)
                    if maze[i][j] == digit_b:
                        b = (i, j)
            distances.update({(digit_a, digit_b): advent.find_grid_distance(grid, a, b)})
    print(advent.find_minimum_time_to_visit_all(digits, distances, '0', False))
    print(advent.find_minimum_time_to_visit_all(digits, distances, '0', True))
