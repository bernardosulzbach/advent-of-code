import advent


def dissect(line):
    name = line[:line.rfind('-')]
    sector = line[line.rfind('-') + 1:line.index('[')]
    checksum = line[line.index('[') + 1:len(line) - 2]
    return name, sector, checksum


def derive_checksum(name):
    frequencies = {}
    for character in name:
        if character != '-':
            frequencies.update({character: frequencies.get(character, 0) + 1})
    values = []
    for key in frequencies:
        values.append((frequencies.get(key), key))
    values = sorted(values, key=lambda v: (v[0], ord('z') - ord(v[1])), reverse=True)
    return ''.join(value[1] for value in values[:4])


def is_real(name, checksum):
    actual_checksum = derive_checksum(name)
    return actual_checksum == checksum


def part_one():
    total = 0
    for line in advent.read_lines(advent.derive_filename(4)):
        name, sector, checksum = dissect(line)
        if is_real(name, checksum):
            total += int(sector)
    print(total)


def shift(name, sector):
    result = []
    for character in name:
        if character == '-':
            result.append(' ')
        else:
            alphabet_size = ord('z') - ord('a') + 1
            result.append(chr(ord('a') + (ord(character) - ord('a') + sector) % alphabet_size))
    return ''.join(result)


def part_two():
    for line in advent.read_lines(advent.derive_filename(4)):
        name, sector, checksum = dissect(line)
        if is_real(name, checksum):
            if shift(name, int(sector)) == 'northpole object storage':
                print(sector)


if __name__ == '__main__':
    part_one()
    part_two()
