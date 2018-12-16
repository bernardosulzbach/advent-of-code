extern crate advent;

use std::vec::Vec;

const WIDTH: usize = 300;

fn evaluate(grid: &Vec<Vec<i64>>, x: usize, y: usize) -> i64 {
    let mut total = 0;
    for i in x..x + 3 {
        for j in y..y + 3 {
            total += grid[i][j];
        }
    }
    return total;
}

fn solve_part_one(grid: &Vec<Vec<i64>>) {
    let mut best_x = 0;
    let mut best_y = 0;
    let mut best_evaluation = std::i64::MIN;
    for x in 0..WIDTH - 2 {
        for y in 0..WIDTH - 2 {
            let evaluation = evaluate(&grid, x, y);
            if evaluation > best_evaluation {
                best_x = x;
                best_y = y;
                best_evaluation = evaluation;
            }
        }
    }
    println!("{},{}", best_x + 1, best_y + 1);
}

fn solve_part_two(grid: &Vec<Vec<i64>>) {
    let mut values = Vec::new();
    for x in 1..WIDTH + 1 {
        values.push(Vec::new());
        for y in 1..WIDTH + 1 {
            values[x - 1].push(Vec::new());
            for _ in 1..std::cmp::min(WIDTH - x + 1, WIDTH - y + 1) + 1 {
                values[x - 1][y - 1].push(0i64);
            }
        }
    }
    for x in (1..WIDTH + 1).rev() {
        for y in (1..WIDTH + 1).rev() {
            for d in 1..std::cmp::min(WIDTH - x + 1, WIDTH - y + 1) + 1 {
                if d == 1 {
                    values[x - 1][y - 1][d - 1] = grid[x - 1][y - 1];
                } else {
                    values[x - 1][y - 1][d - 1] = values[x - 1][y - 1][d - 2];
                    if d > 2 {
                        values[x - 1][y - 1][d - 1] -= values[x][y][d - 3];
                    }
                    values[x - 1][y - 1][d - 1] += grid[x - 1 + d - 1][y - 1];
                    values[x - 1][y - 1][d - 1] += grid[x - 1][y - 1 + d - 1];
                    values[x - 1][y - 1][d - 1] += values[x][y][d - 2];
                }
            }
        }
    }
    let mut best_x = 0;
    let mut best_y = 0;
    let mut best_d = 0;
    let mut best_value = std::i64::MIN;
    for (x, _) in values.iter().enumerate() {
        for (y, _) in values[x].iter().enumerate() {
            for (d, _) in values[x][y].iter().enumerate() {
                if values[x][y][d] > best_value {
                    best_x = x;
                    best_y = y;
                    best_d = d;
                    best_value = values[x][y][d];
                }
            }
        }
    }
    println!("{},{},{}", best_x + 1, best_y + 1, best_d + 1);
}

fn solve(filename: &str) {
    let integers = advent::read_integers_from_file::<i64>(filename);
    for grid_serial_number in integers {
        let mut grid = Vec::new();
        for x in 1..WIDTH + 1 {
            grid.push(Vec::new());
            for y in 1..WIDTH + 1 {
                let rack_id = x as i64 + 10;
                let mut power_level = rack_id * y as i64;
                power_level += grid_serial_number;
                power_level *= rack_id;
                power_level = power_level / 100 % 10;
                power_level -= 5;
                grid[x - 1].push(power_level);
            }
        }
        solve_part_one(&grid);
        solve_part_two(&grid);
    }
}

fn main() {
    solve(&advent::derive_filename(11));
}
