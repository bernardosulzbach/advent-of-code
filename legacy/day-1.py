direction = 0
x = 0
y = 0

with open('day-1.txt', 'r') as f:
    for string in f.read().split():
        string = string.strip(',')
        if (string[0] == 'L'):
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
