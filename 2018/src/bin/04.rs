extern crate advent;

use std::collections::HashMap;
use std::vec::Vec;

fn solve_part_one(sleep_vectors: &HashMap<u16, Vec<u32>>) {
    let mut worst_guard_id = 0u16;
    let mut worst_guard_sleep_sum = 0u32;
    for (guard_id, item) in sleep_vectors.iter() {
        let guard_sleep_sum = item.iter().sum::<u32>();
        if guard_sleep_sum > worst_guard_sleep_sum {
            worst_guard_id = guard_id.clone();
            worst_guard_sleep_sum = guard_sleep_sum;
        }
    }
    let mut worst_entry_i = 0usize;
    let mut worst_entry_value = 0u32;
    for (i, value) in sleep_vectors
        .get(&worst_guard_id)
        .unwrap()
        .iter()
        .enumerate()
    {
        if value > &worst_entry_value {
            worst_entry_i = i;
            worst_entry_value = value.clone();
        }
    }
    println!("{}", worst_guard_id as u64 * worst_entry_i as u64);
}

fn solve_part_two(sleep_vectors: &HashMap<u16, Vec<u32>>) {
    let mut worst_minute = 0usize;
    let mut worst_minute_value = 0u32;
    let mut worst_minute_guard_id = 0u16;
    for (guard_id, value) in sleep_vectors.iter() {
        for (i, value) in value.iter().enumerate() {
            if value > &worst_minute_value {
                worst_minute = i;
                worst_minute_value = value.clone();
                worst_minute_guard_id = guard_id.clone();
            }
        }
    }
    println!("{}", worst_minute_guard_id as u64 * worst_minute as u64);
}

fn solve(filename: &str) {
    let mut lines = advent::read_lines_from_file(filename);
    lines.sort();
    let mut sleep_vectors = HashMap::new();
    let mut current_guard_id = 0u16;
    let mut fall_asleep_minute = 0u8;
    for line in lines.iter() {
        if line.find("#").is_some() {
            let tail = &line[line.find("#").unwrap() + 1..];
            let (number, _) = tail.split_at(tail.find(" ").unwrap());
            current_guard_id = number.parse().unwrap();
        } else {
            let is_falls_asleep = &line[19..20] == "f";
            let minute = line[15..17].parse::<u8>().unwrap();
            if is_falls_asleep {
                fall_asleep_minute = minute;
            } else {
                if !sleep_vectors.contains_key(&current_guard_id) {
                    sleep_vectors.insert(current_guard_id, vec![0; 60]);
                }
                for i in fall_asleep_minute..minute {
                    sleep_vectors.get_mut(&current_guard_id).unwrap()[i as usize] += 1u32;
                }
            }
        }
    }
    solve_part_one(&sleep_vectors);
    solve_part_two(&sleep_vectors);
}

fn main() {
    let filename = advent::derive_filename(4);
    solve(filename.as_ref());
}
