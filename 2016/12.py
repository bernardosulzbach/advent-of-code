import advent


def part_one():
    instructions = []
    for line in advent.read_lines(advent.derive_filename(12)):
        instructions.append(line.split())
    computer = advent.Computer(instructions)
    computer.execute()
    print(computer.get_value('a'))


def part_two():
    instructions = []
    for line in advent.read_lines(advent.derive_filename(12)):
        instructions.append(line.split())
    computer = advent.Computer(instructions)
    computer.registers[computer.get_register_index('c')] = 1
    computer.execute()
    print(computer.get_value('a'))


if __name__ == '__main__':
    part_one()
    part_two()
