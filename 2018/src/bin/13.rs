extern crate advent;

use std::vec::Vec;

struct Rail {
    crash: bool,
    n: bool,
    e: bool,
    s: bool,
    w: bool,
}

impl Rail {
    fn to_char(&self) -> char {
        // 00000
        // 00001
        // 00010
        // 00011 ┐
        // 00100
        // 00101 ─
        // 00110 ┌
        // 00111 ┬
        // 01000
        // 01001 ┘
        // 01010 │
        // 01011 ┤
        // 01100 └
        // 01101 ┴
        // 01110 ├
        // 01111 ┼
        // 10000 ╳
        if self.crash {
            return '╳';
        }
        let index = 0
            | ((self.n as usize) << 3)
            | ((self.e as usize) << 2)
            | ((self.s as usize) << 1)
            | ((self.w as usize) << 0);
        let string = "   ┐ ─┌┬ ┘│┤└┴├┼";
        return string.chars().nth(index).unwrap();
    }
}

fn is_reachable_from_north(grid: &Vec<Vec<Rail>>, i: usize, j: usize) -> bool {
    return i > 0 && grid[i - 1][j].s;
}

fn is_reachable_from_west(grid: &Vec<Vec<Rail>>, i: usize, j: usize) -> bool {
    return j > 0 && grid[i][j - 1].e;
}

fn solve(filename: &str) {
    let lines = advent::read_lines_from_file(filename);
    let mut grid = Vec::new();
    for (i, line) in lines.iter().enumerate() {
        grid.push(Vec::new());
        for (j, character) in line.chars().enumerate() {
            grid.last_mut().unwrap().push(Rail {
                crash: false,
                n: false,
                e: false,
                s: false,
                w: false,
            });
            if character == '-' {
                grid.last_mut().unwrap().last_mut().unwrap().e = true;
                grid.last_mut().unwrap().last_mut().unwrap().w = true;
            } else if character == '|' {
                grid.last_mut().unwrap().last_mut().unwrap().n = true;
                grid.last_mut().unwrap().last_mut().unwrap().s = true;
            } else if character == '/' {
                if is_reachable_from_west(&grid, i, j) {
                    grid.last_mut().unwrap().last_mut().unwrap().n = true;
                    grid.last_mut().unwrap().last_mut().unwrap().w = true;
                } else {
                    grid.last_mut().unwrap().last_mut().unwrap().e = true;
                    grid.last_mut().unwrap().last_mut().unwrap().s = true;
                }
            } else if character == '\\' {
                if is_reachable_from_west(&grid, i, j) {
                    grid.last_mut().unwrap().last_mut().unwrap().s = true;
                    grid.last_mut().unwrap().last_mut().unwrap().w = true;
                } else {
                    grid.last_mut().unwrap().last_mut().unwrap().n = true;
                    grid.last_mut().unwrap().last_mut().unwrap().e = true;
                }
            } else if character == '+' {
                grid.last_mut().unwrap().last_mut().unwrap().n = true;
                grid.last_mut().unwrap().last_mut().unwrap().e = true;
                grid.last_mut().unwrap().last_mut().unwrap().s = true;
                grid.last_mut().unwrap().last_mut().unwrap().w = true;
            } else if character == '>' || character == '<' {
                grid.last_mut().unwrap().last_mut().unwrap().e = true;
                grid.last_mut().unwrap().last_mut().unwrap().w = true;
                if is_reachable_from_north(&grid, i, j) {
                    grid.last_mut().unwrap().last_mut().unwrap().n = true;
                    grid.last_mut().unwrap().last_mut().unwrap().s = true;
                }
            } else if character == 'v' || character == '^' {
                grid.last_mut().unwrap().last_mut().unwrap().n = true;
                grid.last_mut().unwrap().last_mut().unwrap().s = true;
                if is_reachable_from_west(&grid, i, j) {
                    grid.last_mut().unwrap().last_mut().unwrap().e = true;
                    grid.last_mut().unwrap().last_mut().unwrap().w = true;
                }
            }
        }
    }
    for grid_line in grid.iter() {
        for grid_cell in grid_line.iter() {
            print!("{}", grid_cell.to_char());
        }
        println!();
    }
}

fn main() {
    solve(&advent::derive_filename(13));
}
