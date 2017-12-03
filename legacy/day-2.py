code = []
with open('day-2.txt') as f:
    x = 1
    y = 1
    for line in f.readlines():
        for command in line:
            if command == 'U':
                y = max(0, y - 1)
            elif command == 'R':
                x = min(2, x + 1)
            elif command == 'L':
                x = max(0, x - 1)
            elif command == 'D':
                y = min(2, y + 1)
        code.append(str(3 * y + x + 1))
print(''.join(code))
