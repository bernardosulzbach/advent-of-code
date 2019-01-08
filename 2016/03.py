import advent


def is_triangle(integers):
    sides = sorted(integers)
    return sides[0] + sides[1] > sides[2]


def part_one():
    possible = 0
    for line in advent.read_lines(advent.derive_filename(3)):
        if is_triangle(map(int, line.split())):
            possible += 1
    print(possible)


def part_two():
    possible = 0
    buckets = []
    for line in advent.read_lines(advent.derive_filename(3)):
        for i, token in enumerate(line.split()):
            while i >= len(buckets):
                buckets.append([])
            buckets[i].append(int(token))
            if len(buckets[i]) == 3:
                if is_triangle(buckets[i]):
                    possible += 1
                buckets[i].clear()
    print(possible)


if __name__ == '__main__':
    part_one()
    part_two()
