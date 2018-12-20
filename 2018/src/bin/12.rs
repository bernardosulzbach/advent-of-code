extern crate advent;

use std::vec::Vec;

const WINDOW_SIZE: usize = 5;

const PART_ONE_GENERATIONS: u64 = 20;
const PART_TWO_GENERATIONS: u64 = 50_000_000_000;

#[derive(Clone)]
struct State {
    bits: Vec<bool>,
    start: i64,
}

impl State {
    fn normalize(&self) -> State {
        let less_than_window_size = (WINDOW_SIZE - 1) as i64;
        if !self.bits.contains(&true) {
            return State {
                bits: vec![false; (WINDOW_SIZE - 1) + 1 + (WINDOW_SIZE - 1)],
                start: -less_than_window_size,
            };
        }
        let mut found_true = false;
        let mut first_true = 0i64;
        let mut last_true = 0i64;
        for (i, value) in self.bits.iter().enumerate() {
            if *value {
                if !found_true {
                    first_true = i as i64;
                }
                last_true = i as i64;
                found_true = true;
            }
        }
        let mut new_bits = Vec::new();
        for i in first_true - less_than_window_size..last_true + less_than_window_size + 1 {
            if i < 0 || i >= self.bits.len() as i64 {
                new_bits.push(false);
            } else {
                new_bits.push(self.bits[i as usize]);
            }
        }
        return State {
            bits: new_bits,
            start: self.start + first_true - less_than_window_size,
        };
    }
    fn sum_positions(&self) -> i64 {
        let mut sum = 0 as i64;
        for (i, value) in self.bits.iter().enumerate() {
            if *value {
                sum += i as i64 + self.start;
            }
        }
        return sum;
    }
}

fn pattern_to_size(pattern: &str) -> usize {
    let mut result = 0;
    for character in pattern.chars() {
        result = (result << 1) | ((character == '#') as usize);
    }
    return result;
}

fn state_from_string(string: &str) -> State {
    let mut bits = Vec::new();
    for character in string.chars() {
        bits.push(character == '#');
    }
    return State { bits, start: 0 }.normalize();
}

fn evaluate_next_state(state: &State, rules: &Vec<bool>) -> State {
    let mut next_state = Vec::new();
    let mut index = 0usize;
    for (i, value) in state.bits.iter().enumerate() {
        index = (index << 1) | (*value as usize);
        index &= ((1 << WINDOW_SIZE) - 1) as usize;
        if i + 1 >= WINDOW_SIZE {
            next_state.push(rules[index]);
        }
    }
    return State {
        bits: next_state,
        start: state.start + (WINDOW_SIZE / 2) as i64,
    }.normalize();
}

fn evaluate_sum_after_generations(initial_state: &State, rules: &Vec<bool>, generations: u64) {
    let mut current_state = initial_state.clone();
    for generation in 1..generations + 1 {
        let next_state = evaluate_next_state(&current_state, &rules);
        if next_state.bits == current_state.bits {
            println!("Detected drifting at generation {}.", generation);
            let drift = next_state.start - current_state.start;
            let final_start = current_state.start + drift * ((generations - generation + 1) as i64);
            current_state = next_state;
            current_state.start = final_start.clone();
            break;
        } else {
            current_state = next_state;
        }
    }
    println!("Sum is {}.", current_state.sum_positions());
}

fn solve(filename: &str) {
    let lines = advent::read_lines_from_file(filename);
    let mut rules = vec![false; 1 << WINDOW_SIZE];
    for line in lines[2..].iter() {
        let a = &line[..line.find(" => ").unwrap()];
        let b = &line[line.find(" => ").unwrap() + 4..];
        rules[pattern_to_size(a)] = b == "#";
    }
    if rules[0] {
        panic!("Invalid rule for zero.")
    }
    let initial_state_string = &lines[0][lines[0].find(":").unwrap() + 2..];
    let initial_state = state_from_string(initial_state_string);
    evaluate_sum_after_generations(&initial_state, &rules, PART_ONE_GENERATIONS);
    evaluate_sum_after_generations(&initial_state, &rules, PART_TWO_GENERATIONS);
}

fn main() {
    solve(&advent::derive_filename(12));
}
