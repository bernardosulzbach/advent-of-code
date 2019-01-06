use std::collections::hash_map::DefaultHasher;
use std::hash::Hash;
use std::hash::Hasher;
use std::io::prelude::Read;
use std::vec::Vec;

const SAMPLE_PATH: &str = "input/sample/";
const PUZZLE_PATH: &str = "input/puzzle/";

const FILENAME_EXTENSION: &str = ".txt";

pub struct Point<T> {
    pub x: T,
    pub y: T,
}

#[derive(Debug, Copy, Clone, PartialOrd, PartialEq)]
pub enum Direction {
    North,
    East,
    South,
    West,
}

impl Direction {
    pub fn get_symbol(&self) -> char {
        match *self {
            Direction::North => '▲',
            Direction::East => '▶',
            Direction::South => '▼',
            Direction::West => '◀',
        }
    }
    pub fn get_left(&self) -> Direction {
        match *self {
            Direction::North => Direction::West,
            Direction::East => Direction::North,
            Direction::South => Direction::East,
            Direction::West => Direction::South,
        }
    }
    pub fn get_right(&self) -> Direction {
        match *self {
            Direction::North => Direction::East,
            Direction::East => Direction::South,
            Direction::South => Direction::West,
            Direction::West => Direction::North,
        }
    }
    pub fn get_index(&self) -> u32 {
        match *self {
            Direction::North => 0,
            Direction::East => 1,
            Direction::South => 2,
            Direction::West => 3,
        }
    }
}

pub fn derive_filename(day: u32) -> String {
    let mut running_sample = false;
    for (i, argument) in std::env::args().enumerate() {
        if i == 0 {
            continue;
        }
        if argument == "-s" {
            running_sample = true;
        } else {
            panic!("Unrecognized argument.");
        }
    }
    let mut path = PUZZLE_PATH;
    if running_sample {
        path = SAMPLE_PATH;
    }
    return path.to_string() + &format!("{:0>2}", day.to_string()) + FILENAME_EXTENSION;
}

pub fn get_char(string: &str, index: usize) -> char {
    for (i, character) in string.chars().enumerate() {
        if i == index {
            return character;
        }
    }
    panic!("String too short.");
}

pub fn read_integers_from_file<T>(filename: &str) -> Vec<T>
where
    T: std::str::FromStr,
    <T as std::str::FromStr>::Err: std::fmt::Debug,
{
    let mut file = std::fs::File::open(filename).expect("File not found.");
    let mut contents = String::new();
    file.read_to_string(&mut contents)
        .expect("Something went wrong while reading.");
    let mut result = std::vec::Vec::new();
    for word in contents.split_whitespace() {
        result.push(word.parse::<T>().expect("Should have only integers."));
    }
    return result;
}

pub fn read_lines_from_file(filename: &str) -> Vec<String> {
    let mut file = std::fs::File::open(filename).expect("File not found.");
    let mut contents = String::new();
    file.read_to_string(&mut contents)
        .expect("Something went wrong reading the file.");
    let mut result = std::vec::Vec::new();
    for line in contents.lines() {
        result.push(line.to_owned());
    }
    return result;
}

pub fn hash_string(string: &str) -> u64 {
    let mut hasher = DefaultHasher::new();
    string.to_string().hash(&mut hasher);
    return hasher.finish() as u64;
}

pub fn string_difference(a: &str, b: &str) -> usize {
    if a.len() != b.len() {
        panic!("String should have the same size.")
    }
    let mut difference = 0;
    for (from_a, from_b) in a.chars().zip(b.chars()) {
        if from_a != from_b {
            difference += 1;
        }
    }
    return difference;
}

pub fn string_matching(a: &str, b: &str) -> String {
    if a.len() != b.len() {
        panic!("String should have the same size.")
    }
    let mut result = String::new();
    for (from_a, from_b) in a.chars().zip(b.chars()) {
        if from_a == from_b {
            result.push(from_a);
        }
    }
    return result;
}

pub fn string_to_range(string: &str) -> std::ops::Range<i32> {
    if !string.contains("..") {
        let value = string.parse::<i32>().unwrap();
        return value.clone()..value + 1;
    } else {
        let dots = string.find("..").unwrap();
        let start = string[..dots].parse::<i32>().unwrap();
        let end = string[dots + 2..].parse::<i32>().unwrap() + 1;
        return start..end;
    }
}

pub struct Ring<T>
where
    T: std::clone::Clone,
{
    cursor: Option<usize>,
    allocator: usize,
    slots: Vec<Option<RingNode<T>>>,
}

#[derive(Clone)]
struct RingNode<T>
where
    T: std::clone::Clone,
{
    value: T,
    previous: usize,
    next: usize,
}

impl<T> Ring<T>
where
    T: std::clone::Clone,
{
    pub fn new(capacity: usize) -> Self {
        Ring {
            cursor: None,
            allocator: 0,
            slots: vec![None; capacity],
        }
    }

    fn allocate(&mut self) -> usize {
        let mut result = None;
        for offset in 0..self.slots.len() {
            let index = (self.allocator + offset) % self.slots.len();
            if self.slots[index].is_none() {
                result = Some(index);
                break;
            }
        }
        self.allocator = (result.unwrap() + 1) % self.slots.len();
        return result.unwrap();
    }

    fn is_empty(&self) -> bool {
        return self.cursor.is_none();
    }

    fn is_singleton(&self) -> bool {
        if self.is_empty() {
            return false;
        }
        return self.slots[self.cursor.unwrap()].clone().unwrap().next == self.cursor.unwrap();
    }

    pub fn get(&self) -> T {
        return self.slots[self.cursor.unwrap()].clone().unwrap().value;
    }

    pub fn insert(&mut self, value: T) {
        let new_index = self.allocate();
        if self.is_empty() {
            self.slots[new_index] = Some(RingNode {
                value,
                previous: new_index,
                next: new_index,
            });
        } else if self.is_singleton() {
            let current_index = self.cursor.unwrap();
            self.slots[current_index].as_mut().unwrap().previous = new_index;
            self.slots[current_index].as_mut().unwrap().next = new_index;
            self.slots[new_index] = Some(RingNode {
                value,
                previous: current_index,
                next: current_index,
            });
        } else {
            let current_index = self.cursor.unwrap();
            let next_index = self.slots[current_index].clone().unwrap().next;
            self.slots[current_index].as_mut().unwrap().next = new_index;
            self.slots[next_index].as_mut().unwrap().previous = new_index;
            self.slots[new_index] = Some(RingNode {
                value,
                previous: current_index,
                next: next_index,
            });
        }
        self.cursor = Some(new_index);
    }

    pub fn remove(&mut self) -> T {
        if self.is_empty() {
            panic!()
        }
        let current_index = self.cursor.unwrap();
        let value = self.slots[current_index].clone().unwrap().value;
        if self.is_singleton() {
            self.cursor = None;
        } else {
            let previous_index = self.slots[current_index].clone().unwrap().previous;
            let next_index = self.slots[current_index].clone().unwrap().next;
            self.slots[previous_index].as_mut().unwrap().next = next_index;
            self.slots[next_index].as_mut().unwrap().previous = previous_index;
            self.cursor = Some(next_index);
        }
        self.slots[current_index] = None;
        return value;
    }

    pub fn advance(&mut self) {
        self.cursor = Some(self.slots[self.cursor.unwrap()].clone().unwrap().next);
    }

    pub fn go_back(&mut self) {
        let current_index = self.cursor.unwrap();
        self.cursor = Some(self.slots[self.cursor.unwrap()].clone().unwrap().previous);
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn ring_works() {
        let mut ring = Ring::<i32>::new(2);
        ring.insert(1);
        assert_eq!(ring.get(), 1);
        ring.insert(2);
        assert_eq!(ring.get(), 2);
        ring.advance();
        assert_eq!(ring.get(), 1);
        assert_eq!(ring.remove(), 1);
        assert_eq!(ring.get(), 2);
        assert_eq!(ring.remove(), 2);
    }
}
