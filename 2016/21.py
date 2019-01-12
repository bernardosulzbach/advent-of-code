import advent
import itertools


def rotate(password, amount):
    amount %= len(password)
    if amount < 0:
        amount + len(password)
    copy = password.copy()
    for i in range(len(password)):
        password[i] = copy[(i - amount + len(password)) % len(password)]


def apply(password, instruction):
    # swap position X with position Y means that the letters at indexes X and Y (counting from 0) should be swapped.
    split = instruction.split()
    if instruction.startswith('swap position'):
        x = int(split[2])
        y = int(split[5])
        password[x], password[y] = password[y], password[x]
    # swap letter X with letter Y means that the letters X and Y should be swapped.
    if instruction.startswith('swap letter'):
        x = split[2]
        y = split[5]
        for i in range(len(password)):
            if password[i] == x:
                password[i] = y
            elif password[i] == y:
                password[i] = x
    # rotate left/right X steps means that the whole string should be rotated
    if instruction.startswith('rotate left'):
        rotate(password, -int(split[2]))
    if instruction.startswith('rotate right'):
        rotate(password, int(split[2]))
    # rotate based on position of letter X means that the whole string should be rotated to the right based on the index
    # of letter X (counting from 0) as determined before this instruction does any rotations.
    # Once the index is determined, rotate the string to the right one time, plus a number of times equal to that index,
    # plus one additional time if the index was at least 4.
    if instruction.startswith('rotate based on position of letter'):
        index = password.index(split[6])
        amount = 1
        if index >= 4:
            amount += 1
        amount += index
        rotate(password, amount)
    # reverse positions X through Y means that the span of letters at indexes X through Y should be reversed in order.
    if instruction.startswith('reverse positions'):
        x = int(split[2])
        y = int(split[4])
        copy = password.copy()
        for i in range(y - x + 1):
            password[x + i] = copy[y - i]
    # move position X to position Y means that the letter which is at index X should be removed from the string,
    # then inserted such that it ends up at index Y.
    if instruction.startswith('move position'):
        x = int(split[2])
        y = int(split[5])
        password.insert(y, password.pop(x))


def part_one():
    password = list('abcde' if advent.is_using_sample() else 'abcdefgh')
    for instruction in advent.read_lines(advent.derive_filename(21)):
        apply(password, instruction)
    print(''.join(password))


def part_two():
    instructions = advent.read_lines(advent.derive_filename(21))
    for password in itertools.permutations(list('abcdefgh')):
        password = list(password)
        original = password.copy()
        for instruction in instructions:
            apply(password, instruction)
        if password == list('fbgdceah'):
            print(''.join(original))
            break


if __name__ == '__main__':
    part_one()
    part_two()
