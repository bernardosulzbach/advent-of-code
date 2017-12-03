possible = 0
with open('day-3.txt') as f:
    for line in f.readlines():
        sides = sorted(map(int, line.split()))
        if sides[0] + sides[1] > sides[2]:
            possible += 1
print(possible)
