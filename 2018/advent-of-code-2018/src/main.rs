use std::io::prelude::*;

fn read_from_file<T>(filename: &str) -> Vec<T> where
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

fn solve_day_1_part_1(filename: &str) {
    let mut total = 0;
    for integer in read_from_file::<i32>(filename) {
        total += integer;
    }
    println!("{}", total);
}

fn solve_day_1_part_2(filename: &str) {
    let values = read_from_file::<i32>(filename);
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

fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() < 3 {
        println!("Usage: {} [DAY] [FILENAME]", args[0]);
        return;
    }
    let day = args[1].parse::<u32>().expect("Not a valid unsigned integer.");
    if day == 1 {
        solve_day_1_part_1(args[2].as_ref());
        solve_day_1_part_2(args[2].as_ref());
    }
}
