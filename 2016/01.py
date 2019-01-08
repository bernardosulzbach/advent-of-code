import advent


def part_one():
    direction = 0
    x = 0
    y = 0
    text = '\n'.join(advent.read_lines(advent.derive_filename(1)))
    for string in text.split():
        string = string.strip(',')
        if string[0] == 'L':
            direction -= 1
            if direction < 0:
                direction += 4
        else:
            direction += 1
            if direction > 3:
                direction %= 4
        delta = int(''.join(string[1:]))
        if direction == 0:
            y += delta
        elif direction == 1:
            x += delta
        elif direction == 2:
            y -= delta
        else:
            x -= delta
    print(abs(x) + abs(y))


def part_two():
    direction = 0
    x = 0
    y = 0
    text = '\n'.join(advent.read_lines(advent.derive_filename(1)))
    visited = set()
    for string in text.split():
        string = string.strip(',')
        if string[0] == 'L':
            direction -= 1
            if direction < 0:
                direction += 4
        else:
            direction += 1
            if direction > 3:
                direction %= 4
        delta = int(''.join(string[1:]))
        while delta > 0:
            if direction == 0:
                y += 1
            elif direction == 1:
                x += 1
            elif direction == 2:
                y -= 1
            else:
                x -= 1
            delta -= 1
            if (x, y) in visited:
                print(abs(x) + abs(y))
                return
            visited.add((x, y))


if __name__ == '__main__':
    part_one()
    part_two()
