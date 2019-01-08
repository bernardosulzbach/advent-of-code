import hashlib
import advent

base_hash = hashlib.md5()
base_hash.update(bytes(advent.read_lines(advent.derive_filename(5))[0], 'ascii'))

PASSWORD_LENGTH = 8


def part_one():
    password = []
    integer = 0
    while len(password) < PASSWORD_LENGTH:
        hash_object = base_hash.copy()
        hash_object.update(bytes(str(integer), 'utf-8'))
        hexadecimal_digest = hash_object.hexdigest()
        if hexadecimal_digest.startswith('00000'):
            password.append(hexadecimal_digest[5])
        integer += 1
    print(''.join(map(str, password)))


def part_two():
    password = [None] * PASSWORD_LENGTH
    integer = 0
    found = 0
    while found < PASSWORD_LENGTH:
        hash_object = base_hash.copy()
        hash_object.update(bytes(str(integer), 'utf-8'))
        hexadecimal_digest = hash_object.hexdigest()
        if hexadecimal_digest.startswith('00000'):
            index = int(hexadecimal_digest[5], 16)
            if index < PASSWORD_LENGTH and password[index] is None:
                password[index] = hexadecimal_digest[6]
                found += 1
        integer += 1
    print(''.join(map(str, password)))


if __name__ == '__main__':
    part_one()
    part_two()
