import advent


class Keypad:
    def __init__(self, keys, initial_key):
        self.keys = keys
        for i, row in enumerate(self.keys):
            for j, key in enumerate(row):
                if key == initial_key:
                    self.i = i
                    self.j = j

    def peek(self):
        return self.keys[self.i][self.j]

    def digest_character(self, command):
        if command == 'U':
            if self.i > 0 and self.keys[self.i - 1][self.j] != ' ':
                self.i -= 1
        elif command == 'R':
            if self.j + 1 < len(self.keys[self.i]) and self.keys[self.i][self.j + 1] != ' ':
                self.j += 1
        elif command == 'L':
            if self.j > 0 and self.keys[self.i][self.j - 1] != ' ':
                self.j -= 1
        elif command == 'D':
            if self.i + 1 < len(self.keys) and self.keys[self.i + 1][self.j] != ' ':
                self.i += 1

    def digest_line(self, line):
        for character in line:
            self.digest_character(character)
        return self.peek()


def part_one():
    keypad = Keypad([['1', '2', '3'], ['4', '5', '6'], ['7', '8', '9']], '5')
    keys = []
    for line in advent.read_lines(advent.derive_filename(2)):
        keys.append(keypad.digest_line(line))
    print(''.join(keys))


def part_two():
    keypad = Keypad([[' ', ' ', '1', ' ', ' '],
                     [' ', '2', '3', '4', ' '],
                     ['5', '6', '7', '8', '9'],
                     [' ', 'A', 'B', 'C', ' '],
                     [' ', ' ', 'D', ' ', ' ']], '5')
    keys = []
    for line in advent.read_lines(advent.derive_filename(2)):
        keys.append(keypad.digest_line(line))
    print(''.join(keys))


if __name__ == '__main__':
    part_one()
    part_two()
