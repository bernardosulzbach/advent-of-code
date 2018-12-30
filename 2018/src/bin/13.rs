extern crate advent;

use std::vec::Vec;

#[derive(Copy, Clone)]
struct Cart {
    moved: bool,
    turn_modulo: u8,
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

    fn is_crossroads(&self) -> bool {
        return self.n && self.e && self.s && self.w;
    }
}

struct Grid {
    collisions: Vec<advent::Point<usize>>,
    matrix: Vec<Vec<Rail>>,
}

impl Grid {
    fn new(lines: &Vec<String>) -> Grid {
        let mut grid = Grid {
            collisions: Vec::new(),
            matrix: Vec::new(),
        };
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
                            moved: false,
                            turn_modulo: 0,
                            direction: advent::Direction::East,
                        };
                        let grid_row = grid.matrix.last_mut().unwrap();
                        grid_row.last_mut().unwrap().carts.push(cart);
                    } else {
                        let cart = Cart {
                            moved: false,
                            turn_modulo: 0,
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
                            moved: false,
                            turn_modulo: 0,
                            direction: advent::Direction::South,
                        };
                        let grid_row = grid.matrix.last_mut().unwrap();
                        grid_row.last_mut().unwrap().carts.push(cart);
                    } else {
                        let cart = Cart {
                            moved: false,
                            turn_modulo: 0,
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

    fn step(&mut self) {
        for old_i in 0..self.matrix.len() {
            for old_j in 0..self.matrix[old_i].len() {
                if self.matrix[old_i][old_j].carts.len() == 1 {
                    let mut cart = self.matrix[old_i][old_j].carts[0];
                    if cart.moved {
                        continue;
                    }
                    let direction = cart.direction;
                    let mut new_i = old_i;
                    let mut new_j = old_j;
                    if direction == advent::Direction::North {
                        new_i -= 1;
                    } else if direction == advent::Direction::East {
                        new_j += 1;
                    } else if direction == advent::Direction::South {
                        new_i += 1;
                    } else if direction == advent::Direction::West {
                        new_j -= 1;
                    }
                    if self.matrix[new_i][new_j].is_crossroads() {
                        if cart.turn_modulo == 0 {
                            cart.direction = cart.direction.get_left();
                        } else if self.matrix[old_i][old_j].carts[0].turn_modulo == 2 {
                            cart.direction = cart.direction.get_right();
                        }
                        cart.turn_modulo = (cart.turn_modulo + 1) % 3;
                    } else {
                        let destination = &self.matrix[new_i][new_j];
                        let open_n = destination.n && cart.direction == advent::Direction::North;
                        let open_e = destination.e && cart.direction == advent::Direction::East;
                        let open_s = destination.s && cart.direction == advent::Direction::South;
                        let open_w = destination.w && cart.direction == advent::Direction::West;
                        if !(open_n || open_e || open_s || open_w) {
                            if destination.n && cart.direction != advent::Direction::South {
                                cart.direction = advent::Direction::North;
                            } else if destination.e && cart.direction != advent::Direction::West {
                                cart.direction = advent::Direction::East;
                            } else if destination.s && cart.direction != advent::Direction::North {
                                cart.direction = advent::Direction::South;
                            } else if destination.w && cart.direction != advent::Direction::East {
                                cart.direction = advent::Direction::West;
                            }
                        }
                    }
                    cart.moved = true;
                    self.matrix[old_i][old_j].carts.clear();
                    self.matrix[new_i][new_j].carts.push(cart);
                    if self.matrix[new_i][new_j].carts.len() >= 2 {
                        self.collisions.push(advent::Point { x: new_j, y: new_i });
                        self.matrix[new_i][new_j].carts.clear();
                    }
                }
            }
        }
        for i in 0..self.matrix.len() {
            for j in 0..self.matrix[0].len() {
                for cart in self.matrix[i][j].carts.iter_mut() {
                    cart.moved = false;
                }
            }
        }
    }

    fn get_car_count(&self) -> usize {
        let mut count = 0;
        for i in 0..self.matrix.len() {
            for j in 0..self.matrix[0].len() {
                count += self.matrix[i][j].carts.len();
            }
        }
        return count;
    }

    fn get_last_cart_position(&self) -> advent::Point<usize> {
        for i in 0..self.matrix.len() {
            for j in 0..self.matrix[0].len() {
                if self.matrix[i][j].carts.len() == 1 {
                    return advent::Point { x: j, y: i };
                }
            }
        }
        panic!("No cart found.")
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
    while grid.get_car_count() > 1 {
        grid.step();
    }
    println!("{},{}", grid.collisions[0].x, grid.collisions[0].y);
    let last_car_position = grid.get_last_cart_position();
    println!("{},{}", last_car_position.x, last_car_position.y);
}

fn main() {
    solve(&advent::derive_filename(13));
}
