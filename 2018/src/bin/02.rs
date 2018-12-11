extern crate advent;

use std::collections::HashSet;
use std::vec::Vec;

fn solve_first_part(filename: &str) {
    let mut count_2 = 0;
    let mut count_3 = 0;
    for line in advent::read_lines_from_file(filename) {
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
        if has_2 {
            count_2 += 1;
        }
        if has_3 {
            count_3 += 1;
        }
    }
    println!("{}", count_2 * count_3);
}

fn solve_second_part(filename: &str) {
    // Two strings that differ only in a single character will either be equal in one of the halves.
    let lines = advent::read_lines_from_file(filename);
    let mut buckets = std::collections::HashMap::new();
    for (i, line) in lines.iter().enumerate() {
        let (part_a, part_b) = line.split_at(line.len() / 2);
        let hash_a = advent::hash_string(part_a);
        let hash_b = advent::hash_string(part_b);
        if !buckets.contains_key(&hash_a) {
            buckets.insert(hash_a, HashSet::new());
        }
        if !buckets.contains_key(&hash_b) {
            buckets.insert(hash_b, HashSet::new());
        }
        buckets.get_mut(&hash_a).unwrap().insert(i);
        buckets.get_mut(&hash_a).unwrap().insert(i);
    }
    for (_, set) in buckets.iter() {
        let mut vector: Vec<usize> = Vec::with_capacity(set.len());
        vector.extend(set.iter());
        for i in 0..vector.len() {
            for j in i + 1..vector.len() {
                let a_reference = lines[vector[i]].as_ref();
                let b_reference = lines[vector[j]].as_ref();
                if advent::string_difference(a_reference, b_reference) == 1 {
                    println!("{}", advent::string_matching(a_reference, b_reference));
                }
            }
        }
    }
}

fn solve(filename: &str) {
    solve_first_part(filename);
    solve_second_part(filename);
}

fn main() {
    solve(&advent::derive_filename(2));
}
