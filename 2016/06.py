import advent
import collections


def part_one():
    counters = []
    for line in advent.read_lines(advent.derive_filename(6)):
        while len(counters) < len(line):
            counters.append(collections.Counter())
        for i, character in enumerate(line):
            counters[i][character] += 1
    answer = []
    for counter in counters:
        answer.append(counter.most_common()[0][0])
    print(''.join(answer))


def part_two():
    counters = []
    for line in advent.read_lines(advent.derive_filename(6)):
        while len(counters) < len(line):
            counters.append(collections.Counter())
        for i, character in enumerate(line):
            counters[i][character] += 1
    answer = []
    for counter in counters:
        answer.append(counter.most_common()[-1][0])
    print(''.join(answer))


if __name__ == '__main__':
    part_one()
    part_two()
