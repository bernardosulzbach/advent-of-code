use std::collections::hash_map::DefaultHasher;
use std::hash::Hash;
use std::hash::Hasher;
use std::io::prelude::Read;
use std::vec::Vec;

const FILENAME_PATH: &str = "input/";
const FILENAME_EXTENSION: &str = ".txt";

pub fn derive_filename(day: u32) -> String {
    return FILENAME_PATH.to_string() + &day.to_string() + FILENAME_EXTENSION;
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
    for line in contents.lines() {
        result.push(line.parse::<T>().expect("Should have only integers."));
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
