extern crate advent;

use std::vec::Vec;

struct Cart {
    direction: advent::Direction,
}

struct Rail {
    n: bool,
    e: bool,
    s: bool,
    w: bool,
    carts: Vec<Cart>,
}

impl Rail {
    fn to_char(&self) -> char {
        // 00000
        // 00001 ?
        // 00010 ?
        // 00011 ┐
        // 00100 ?
        // 00101 ─
        // 00110 ┌
        // 00111 ┬
        // 01000 ?
        // 01001 ┘
        // 01010 │
        // 01011 ┤
        // 01100 └
        // 01101 ┴
        // 01110 ├
        // 01111 ┼
        // 10000 ╳
        if self.carts.len() >= 2 {
            return '╳';
        }
        if self.carts.len() == 1 {
            return self.carts.first().unwrap().direction.get_symbol();
        }
        let index = 0
            | ((self.n as usize) << 3)
            | ((self.e as usize) << 2)
            | ((self.s as usize) << 1)
            | ((self.w as usize) << 0);
        let string = " ??┐?─┌┬ ┘│┤└┴├┼";
        return string.chars().nth(index).unwrap();
    }
}

struct Grid {
    matrix: Vec<Vec<Rail>>,
}

impl Grid {
    fn new(lines: &Vec<String>) -> Grid {
        let mut grid = Grid { matrix: Vec::new() };
        for (i, line) in lines.iter().enumerate() {
            grid.matrix.push(Vec::new());
            for (j, character) in line.chars().enumerate() {
                grid.matrix.last_mut().unwrap().push(Rail {
                    n: false,
                    e: false,
                    s: false,
                    w: false,
                    carts: Vec::new(),
                });
                if character == '-' {
                    grid.matrix.last_mut().unwrap().last_mut().unwrap().e = true;
                    grid.matrix.last_mut().unwrap().last_mut().unwrap().w = true;
                } else if character == '|' {
                    grid.matrix.last_mut().unwrap().last_mut().unwrap().n = true;
                    grid.matrix.last_mut().unwrap().last_mut().unwrap().s = true;
                } else if character == '/' {
                    if grid.is_reachable_from_west(i, j) {
                        grid.matrix.last_mut().unwrap().last_mut().unwrap().n = true;
                        grid.matrix.last_mut().unwrap().last_mut().unwrap().w = true;
                    } else {
                        grid.matrix.last_mut().unwrap().last_mut().unwrap().e = true;
                        grid.matrix.last_mut().unwrap().last_mut().unwrap().s = true;
                    }
                } else if character == '\\' {
                    if grid.is_reachable_from_west(i, j) {
                        grid.matrix.last_mut().unwrap().last_mut().unwrap().s = true;
                        grid.matrix.last_mut().unwrap().last_mut().unwrap().w = true;
                    } else {
                        grid.matrix.last_mut().unwrap().last_mut().unwrap().n = true;
                        grid.matrix.last_mut().unwrap().last_mut().unwrap().e = true;
                    }
                } else if character == '+' {
                    grid.matrix.last_mut().unwrap().last_mut().unwrap().n = true;
                    grid.matrix.last_mut().unwrap().last_mut().unwrap().e = true;
                    grid.matrix.last_mut().unwrap().last_mut().unwrap().s = true;
                    grid.matrix.last_mut().unwrap().last_mut().unwrap().w = true;
                } else if character == '>' || character == '<' {
                    grid.matrix.last_mut().unwrap().last_mut().unwrap().e = true;
                    grid.matrix.last_mut().unwrap().last_mut().unwrap().w = true;
                    if grid.is_reachable_from_north(i, j) {
                        grid.matrix.last_mut().unwrap().last_mut().unwrap().n = true;
                        grid.matrix.last_mut().unwrap().last_mut().unwrap().s = true;
                    }
                    if character == '>' {
                        let cart = Cart {
                            direction: advent::Direction::East,
                        };
                        let grid_row = grid.matrix.last_mut().unwrap();
                        grid_row.last_mut().unwrap().carts.push(cart);
                    } else {
                        let cart = Cart {
                            direction: advent::Direction::West,
                        };
                        let grid_row = grid.matrix.last_mut().unwrap();
                        grid_row.last_mut().unwrap().carts.push(cart);
                    }
                } else if character == 'v' || character == '^' {
                    grid.matrix.last_mut().unwrap().last_mut().unwrap().n = true;
                    grid.matrix.last_mut().unwrap().last_mut().unwrap().s = true;
                    if grid.is_reachable_from_west(i, j) {
                        grid.matrix.last_mut().unwrap().last_mut().unwrap().e = true;
                        grid.matrix.last_mut().unwrap().last_mut().unwrap().w = true;
                    }
                    if character == 'v' {
                        let cart = Cart {
                            direction: advent::Direction::South,
                        };
                        let grid_row = grid.matrix.last_mut().unwrap();
                        grid_row.last_mut().unwrap().carts.push(cart);
                    } else {
                        let cart = Cart {
                            direction: advent::Direction::North,
                        };
                        let grid_row = grid.matrix.last_mut().unwrap();
                        grid_row.last_mut().unwrap().carts.push(cart);
                    }
                }
            }
        }
        return grid;
    }

    fn print(&self) {
        for matrix_row in self.matrix.iter() {
            for cell in matrix_row.iter() {
                print!("{}", cell.to_char());
            }
            println!();
        }
    }

    fn is_reachable_from_north(&self, i: usize, j: usize) -> bool {
        return i > 0 && self.matrix[i - 1][j].s;
    }

    fn is_reachable_from_west(&self, i: usize, j: usize) -> bool {
        return j > 0 && self.matrix[i][j - 1].e;
    }
}

fn solve(filename: &str) {
    let lines = advent::read_lines_from_file(filename);
    let mut grid = Grid::new(&lines);
    grid.print();
}

fn main() {
    solve(&advent::derive_filename(13));
}
