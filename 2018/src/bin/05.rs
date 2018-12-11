extern crate advent;

use std::vec::Vec;

const NONE: usize = std::usize::MAX;

// A structure which can be used when building pre-allocated linked lists.
struct LinkedListNode {
    value: u8,
    back: usize,
    next: usize,
}

impl LinkedListNode {
    fn new(value: u8) -> LinkedListNode {
        return LinkedListNode {
            value,
            back: NONE,
            next: NONE,
        };
    }
}

fn should_be_destroyed(a: u8, b: u8) -> bool {
    return std::cmp::max(a, b) - std::cmp::min(a, b) == ('a' as u8 - 'A' as u8);
}

fn reduce(line: &str) -> u32 {
    let mut bytes = Vec::new();
    for (i, character) in line.chars().enumerate() {
        bytes.push(LinkedListNode::new(character as u8));
        if i > 0 {
            bytes[i - 1].next = i;
            bytes[i].back = i - 1;
        }
    }
    let mut i = 0usize;
    while i != NONE && bytes[i].next != NONE {
        let j = bytes[i].next;
        if should_be_destroyed(bytes[i].value, bytes[j].value) {
            let mut next_i = NONE;
            bytes[i].value = ' ' as u8;
            bytes[j].value = ' ' as u8;
            if bytes[i].back != NONE {
                let back = bytes[i].back;
                next_i = back;
                bytes[back].next = bytes[j].next;
            }
            if bytes[j].next != NONE {
                let next = bytes[j].next;
                if next_i == NONE {
                    next_i = next;
                }
                bytes[next].back = bytes[i].back;
            }
            i = next_i;
        } else {
            i = bytes[i].next;
        }
    }
    let mut count = 0u32;
    for byte in bytes {
        if byte.value != ' ' as u8 {
            count += 1;
        }
    }
    return count;
}

fn solve(filename: &str) {
    let lines = advent::read_lines_from_file(filename);
    for line in lines.iter() {
        println!("{}", reduce(&line));
        let mut best_reduction_result = std::u32::MAX;
        for ignored_character in 'a' as u8..'z' as u8 {
            let mut clean_line = String::new();
            for character in line.chars() {
                if character.to_ascii_lowercase() as u8 != ignored_character {
                    clean_line.push(character);
                }
            }
            best_reduction_result = std::cmp::min(best_reduction_result, reduce(&clean_line));
        }
        println!("{}", best_reduction_result)
    }
}

fn main() {
    solve(&advent::derive_filename(5));
}
