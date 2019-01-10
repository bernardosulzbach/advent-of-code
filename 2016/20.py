import advent
import collections


def merge_intervals(intervals):
    result = []
    for interval in intervals:
        if result and result[-1][1] + 1 >= interval[0]:
            result[-1][1] = max(result[-1][1], interval[1])
        else:
            result.append(interval)
    return result


def part_one():
    intervals = []
    for line in advent.read_lines(advent.derive_filename(20)):
        parts = line.split('-')
        intervals.append([int(parts[0]), int(parts[1])])
    intervals = merge_intervals(list(sorted(intervals)))
    value = 0
    for interval in intervals:
        if interval[0] > value:
            break
        value = interval[1] + 1
    print(value)


def part_two():
    intervals = []
    for line in advent.read_lines(advent.derive_filename(20)):
        parts = line.split('-')
        intervals.append([int(parts[0]), int(parts[1])])
    intervals = merge_intervals(list(sorted(intervals)))
    allowed = 2 ** 32
    for interval in intervals:
        allowed -= interval[1] - interval[0] + 1
    print(allowed)


if __name__ == '__main__':
    part_one()
    part_two()
