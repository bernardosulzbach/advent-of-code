extern crate advent;

use std::vec::Vec;

#[derive(Copy, Clone)]
enum State {
    Sand,
    Clay,
    WetSand,
    Water,
}

impl State {
    fn to_char(&self) -> char {
        match self {
            State::Sand => '.',
            State::Clay => '#',
            State::WetSand => '|',
            State::Water => '~',
        }
    }
}

fn solve(filename: &str) {
    let lines = advent::read_lines_from_file(filename);
    let mut ranges = Vec::new();
    for line in lines.iter() {
        let left_begin = line.find('=').unwrap() + 1;
        let left_end = line.find(',').unwrap();
        let right_begin = line[left_end..].find('=').unwrap() + left_end + 1;
        let right_end = line.len();
        let left_string = line[left_begin..left_end].as_ref();
        let right_string = line[right_begin..right_end].as_ref();
        let left_range = advent::string_to_range(left_string);
        let right_range = advent::string_to_range(right_string);
        if line.chars().nth(0).unwrap() == 'x' {
            ranges.push((left_range, right_range));
        } else {
            ranges.push((right_range, left_range));
        }
    }
    let mut min_x = std::i32::MAX;
    let mut min_y = std::i32::MAX;
    let mut max_x = std::i32::MIN;
    let mut max_y = std::i32::MIN;
    for (x_range, y_range) in ranges.iter() {
        for x in x_range.clone() {
            min_x = std::cmp::min(min_x, x);
            max_x = std::cmp::max(max_x, x);
            for y in y_range.clone() {
                min_y = std::cmp::min(min_y, y);
                max_y = std::cmp::max(max_y, y);
            }
        }
    }
    let mut grid = Vec::new();
    for row in min_y..max_y + 1 {
        grid.push(vec![State::Sand; (max_x - min_x + 3) as usize]);
    }
    for (x_range, y_range) in ranges.iter() {
        for x in x_range.clone() {
            for y in y_range.clone() {
                grid[(y - min_y) as usize][(x - min_x + 1) as usize] = State::Clay;
            }
        }
    }
    for row in grid.iter() {
        for state in row.iter() {
            print!("{}", state.to_char());
        }
        println!();
    }
}

fn main() {
    solve(&advent::derive_filename(17));
}
