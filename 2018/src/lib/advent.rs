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
        if *self == Direction::North {
            return '▲';
        } else if *self == Direction::East {
            return '▶';
        } else if *self == Direction::South {
            return '▼';
        } else if *self == Direction::West {
            return '◀';
        }
        panic!("Unhandled.");
    }
    pub fn get_left(&self) -> Direction {
        if *self == Direction::North {
            return Direction::West;
        } else if *self == Direction::East {
            return Direction::North;
        } else if *self == Direction::South {
            return Direction::East;
        } else if *self == Direction::West {
            return Direction::South;
        }
        panic!("Unhandled.");
    }
    pub fn get_right(&self) -> Direction {
        if *self == Direction::North {
            return Direction::East;
        } else if *self == Direction::East {
            return Direction::South;
        } else if *self == Direction::South {
            return Direction::West;
        } else if *self == Direction::West {
            return Direction::North;
        }
        panic!("Unhandled.");
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
