import advent

def part_one():
    text = ''.join(advent.read_lines(advent.derive_filename(9)))
    result = []
    multiplier = 1
    capture = []
    required_capture = 0
    index = 0
    while index < len(text):
        character = text[index]
        if len(capture) < required_capture:
            capture.append(character)
            if len(capture) == required_capture:
                result.append(''.join(capture) * multiplier)
                capture.clear()
                required_capture = 0
        elif character == '(':
            closing_parenthesis_index = text.find(')', index)
            parts = text[index + 1:closing_parenthesis_index].split('x')
            required_capture = int(parts[0])
            multiplier = int(parts[1])
            index = closing_parenthesis_index
        else:
            result.append(character)
        index += 1
    resulting_string = ''.join(result)
    print(len(resulting_string))


def length_after_expanding(text):
    length = 0
    index = 0
    while index < len(text):
        character = text[index]
        if character == '(':
            closing_parenthesis_index = text.find(')', index)
            parts = text[index + 1:closing_parenthesis_index].split('x')
            required_capture = int(parts[0])
            multiplier = int(parts[1])
            second_start = closing_parenthesis_index + 1
            length += multiplier * length_after_expanding(text[second_start:second_start + required_capture])
            index = second_start + required_capture-1
        else:
            length += 1
        index += 1
    return length


def part_two():
    text = ''.join(advent.read_lines(advent.derive_filename(9)))
    print(length_after_expanding(text))


if __name__ == '__main__':
    part_one()
    part_two()
