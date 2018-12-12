extern crate advent;

use std::collections::HashMap;
use std::collections::HashSet;
use std::vec::Vec;

fn get_metadata_sum(integers: &Vec<u32>, start: usize, sum: &mut u32) -> usize {
    if start == integers.len() {
        panic!();
    }
    let children = integers[start] as usize;
    let metadata = integers[start + 1] as usize;
    let mut position = start + 2;
    for child in 0..children {
        position = get_metadata_sum(integers, position, sum);
    }
    for i in position..position + metadata {
        *sum += integers[i];
    }
    return position + metadata;
}

fn get_node_value(integers: &Vec<u32>, start: usize, value: &mut u32) -> usize {
    if start == integers.len() {
        panic!();
    }
    let children = integers[start] as usize;
    let metadata = integers[start + 1] as usize;
    let mut position = start + 2;
    if children > 0 {
        let mut children_value = vec![0u32; children];
        for child in 0..children {
            position = get_node_value(integers, position, children_value.get_mut(child).unwrap());
        }
        for i in position..position + metadata {
            if integers[i] <= children as u32 {
                *value += children_value[integers[i] as usize - 1];
            }
        }
    } else {
        for i in position..position + metadata {
            *value += integers[i];
        }
    }
    return position + metadata;
}

fn solve(filename: &str) {
    let integers = advent::read_integers_from_file::<u32>(filename);
    let mut sum = 0;
    get_metadata_sum(&integers, 0, &mut sum);
    println!("{}", sum);
    let mut value = 0;
    get_node_value(&integers, 0, &mut value);
    println!("{}", value);
}

fn main() {
    solve(&advent::derive_filename(8));
}
