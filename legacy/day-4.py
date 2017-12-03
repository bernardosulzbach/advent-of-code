def dissect(line):
    name = line[:line.rfind('-')]
    sector = line[line.rfind('-') + 1:line.index('[')]
    checksum = line[line.index('[') + 1:len(line) - 2]
    return name, sector, checksum


def derive_checksum(name):
    frequencies = {}
    for c in name:
        if c != '-':
            frequencies.update({c: frequencies.get(c, 0) + 1})
    values = []
    for key in frequencies:
        values.append((frequencies.get(key), key))
    values = sorted(values, key=lambda v: (v[0], ord('z') - ord(v[1])), reverse=True)
    return ''.join(value[1] for value in values[:5])


def is_real(name, checksum):
    actual_checksum = derive_checksum(name)
    return actual_checksum == checksum


with open('day-4.txt') as f:
    total = 0
    for line in f.readlines():
        name, sector, checksum = dissect(line)
        if is_real(name, checksum):
            total += int(sector)
print(total)
