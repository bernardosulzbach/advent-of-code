extern crate advent;

use std::collections::HashMap;
use std::collections::HashSet;
use std::vec::Vec;

const WORKERS: u32 = 5;
const NO_STEP: char = '.';

fn get_char(string: &str, index: usize) -> char {
    for (i, character) in string.chars().enumerate() {
        if i == index {
            return character;
        }
    }
    panic!("String too short.");
}

fn solve_part_one(steps: &HashSet<char>, mut blockers: HashMap<char, HashSet<char>>) {
    let mut ordered_steps = Vec::new();
    for value in steps.iter() {
        ordered_steps.push(*value);
    }
    ordered_steps.sort();
    let mut sequence = String::new();
    while sequence.len() < ordered_steps.len() {
        for character in ordered_steps.iter() {
            if sequence.contains(*character) {
                continue;
            }
            if blockers.get(&character).unwrap().is_empty() {
                sequence.push(*character);
                for blocker in blockers.values_mut() {
                    blocker.remove(character);
                }
                break;
            }
        }
    }
    println!("{}", sequence);
}

fn take_available(available: &mut HashSet<char>) -> char {
    if available.is_empty() {
        panic!("Available should not be empty.")
    }
    let mut minimum_available = 'Z';
    for available_step in available.iter() {
        if (*available_step as u8) < (minimum_available as u8) {
            minimum_available = *available_step;
        }
    }
    available.remove(&minimum_available);
    return minimum_available;
}

fn solve_part_two(mut steps: HashSet<char>, mut blockers: HashMap<char, HashSet<char>>) {
    struct Worker {
        step: char,
        time: u32,
    }
    let original_step_count = steps.len();
    let mut workers = Vec::new();
    for _ in 0..WORKERS {
        workers.push(Worker {
            step: NO_STEP,
            time: 0,
        });
    }
    let mut available = HashSet::new();
    let mut finished = 0;
    let mut seconds = 0;
    loop {
        for worker in workers.iter_mut() {
            if worker.time > 0 {
                worker.time -= 1;
            }
            if worker.time == 0 && worker.step != NO_STEP {
                for blocker in blockers.values_mut() {
                    blocker.remove(&worker.step);
                }
                worker.step = NO_STEP;
                finished += 1;
            }
        }
        if finished == original_step_count {
            break;
        }
        let mut newly_available = Vec::new();
        for step in steps.iter() {
            if blockers.get(step).unwrap().is_empty() {
                newly_available.push(*step);
            }
        }
        for step in newly_available.iter() {
            steps.remove(step);
            available.insert(*step);
        }
        for worker in workers.iter_mut() {
            if worker.step == NO_STEP && !available.is_empty() {
                worker.step = take_available(&mut available);
                worker.time = 60 + (worker.step as u8 - 'A' as u8 + 1) as u32;
            }
        }
        seconds += 1;
    }
    println!("{}", seconds)
}

fn solve(filename: &str) {
    let mut blockers = HashMap::new();
    for character in 'A' as u8..('Z' as u8 + 1) {
        blockers.insert(character as char, HashSet::new());
    }
    let mut steps = HashSet::new();
    let lines = advent::read_lines_from_file(filename);
    for line in lines.iter() {
        let a = get_char(&line.as_str(), 5);
        let b = get_char(&line.as_str(), 36);
        steps.insert(a);
        steps.insert(b);
        if !blockers.contains_key(&b) {
            blockers.insert(b, HashSet::new());
        }
        blockers.get_mut(&b).unwrap().insert(a);
    }
    solve_part_one(&steps, blockers.clone());
    solve_part_two(steps.clone(), blockers.clone());
}

fn main() {
    let filename = advent::derive_filename(7);
    solve(filename.as_ref());
}
