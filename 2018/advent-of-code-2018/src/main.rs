use std::io::prelude::*;
use std::collections::HashSet;
use std::hash::Hash;
use std::vec::Vec;
use std::iter::FromIterator;
use std::hash::Hasher;
use std::collections::hash_map::DefaultHasher;

const FILENAME_PATH: &str = "input/";
const FILENAME_EXTENSION: &str = ".txt";

fn read_integers_from_file<T>(filename: &str) -> Vec<T> where
    T: std::str::FromStr,
    <T as std::str::FromStr>::Err: std::fmt::Debug
{
    let mut file = std::fs::File::open(filename).expect("File not found.");
    let mut contents = String::new();
    file.read_to_string(&mut contents).expect("Something went wrong reading the file.");
    let mut result = std::vec::Vec::new();
    for line in contents.lines() {
        result.push(line.parse::<T>().expect("Should have only integers."));
    }
    return result;
}

fn read_lines_from_file(filename: &str) -> Vec<String> {
    let mut file = std::fs::File::open(filename).expect("File not found.");
    let mut contents = String::new();
    file.read_to_string(&mut contents).expect("Something went wrong reading the file.");
    let mut result = std::vec::Vec::new();
    for line in contents.lines() {
        result.push(line.to_owned());
    }
    return result;
}

fn solve_day_1_part_1(filename: &str) {
    let mut total = 0;
    for integer in read_integers_from_file::<i32>(filename) {
        total += integer;
    }
    println!("{}", total);
}

fn solve_day_1_part_2(filename: &str) {
    let values = read_integers_from_file::<i32>(filename);
    let mut seen = std::collections::HashSet::new();
    let mut total = 0;
    loop {
        for value in values.iter() {
            total += value;
            if seen.contains(&total) {
                println!("{}", total);
                return;
            }
            seen.insert(total);
        }
    }
}

fn solve_day_2_part_1(filename: &str) {
    let mut count_2 = 0;
    let mut count_3 = 0;
    for line in read_lines_from_file(filename) {
        let mut counters = [0; 256];
        for character in line.chars() {
            counters[(character as usize) % 256] += 1;
        }
        let mut has_2 = false;
        let mut has_3 = false;
        for count in counters.iter() {
            has_2 = has_2 || count.to_owned() == 2;
            has_3 = has_3 || count.to_owned() == 3;
        }
        if has_2 { count_2 += 1; }
        if has_3 { count_3 += 1; }
    }
    println!("{}", count_2 * count_3);
}

fn solve_day_2_part_2_naive(_filename: &str) {}

fn hash_string(string: &str) -> u64 {
    let mut hasher = DefaultHasher::new();
    string.to_string().hash(&mut hasher);
    return hasher.finish() as u64;
}

fn string_difference(a: &str, b: &str) -> usize {
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

fn string_matching(a: &str, b: &str) -> String {
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

fn solve_day_2_part_2_fast(filename: &str) {
    // Two strings that differ only in a single character will either be equal in one of the halves.
    let lines = read_lines_from_file(filename);
    let mut buckets = std::collections::HashMap::new();
    for (i, line) in lines.iter().enumerate() {
        let (part_a, part_b) = line.split_at(line.len() / 2);
        let hash_a = hash_string(part_a);
        let hash_b = hash_string(part_b);
        if !buckets.contains_key(&hash_a) { buckets.insert(hash_a, HashSet::new()); }
        if !buckets.contains_key(&hash_b) { buckets.insert(hash_b, HashSet::new()); }
        buckets.get_mut(&hash_a).unwrap().insert(i);
        buckets.get_mut(&hash_a).unwrap().insert(i);
    }
    for (hash, set) in buckets.iter() {
        let mut vector: Vec<usize> = Vec::with_capacity(set.len());
        vector.extend(set.iter());
        for i in 0..vector.len() {
            for j in i + 1..vector.len() {
                let a_reference = lines[vector[i]].as_ref();
                let b_reference = lines[vector[j]].as_ref();
                if string_difference(a_reference, b_reference) == 1 {
                    println!("{}", string_matching(a_reference, b_reference));
                }
            }
        }
    }
}


fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() != 2 {
        println!("Usage: {} [DAY]", args[0]);
        return;
    }
    let day = args[1].parse::<u32>().expect("Not a valid unsigned integer.");
    let filename = FILENAME_PATH.to_string() + &day.to_string() + FILENAME_EXTENSION;
    if day == 1 {
        solve_day_1_part_1(filename.as_ref());
        solve_day_1_part_2(filename.as_ref());
    } else if day == 2 {
        solve_day_2_part_1(filename.as_ref());
        solve_day_2_part_2_fast(filename.as_ref());
    }
}
