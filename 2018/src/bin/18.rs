extern crate advent;

use std::vec::Vec;

const OPEN_GROUND_CHARACTER: char = '.';
const TREES_CHARACTER: char = '|';
const LUMBERYARD_CHARACTER: char = '#';

#[derive(Copy, Clone, PartialOrd, PartialEq)]
enum SoilType {
    OpenGround,
    Trees,
    Lumberyard,
}

impl SoilType {
    fn to_char(&self) -> char {
        match self {
            SoilType::OpenGround => OPEN_GROUND_CHARACTER,
            SoilType::Trees => TREES_CHARACTER,
            SoilType::Lumberyard => LUMBERYARD_CHARACTER,
        }
    }
}

#[derive(Copy, Clone, PartialOrd, PartialEq)]
struct Cell {
    soil: SoilType,
}

impl Cell {
    fn from_char(character: char) -> Cell {
        if character == OPEN_GROUND_CHARACTER {
            return Cell {
                soil: SoilType::OpenGround,
            };
        } else if character == TREES_CHARACTER {
            return Cell {
                soil: SoilType::Trees,
            };
        } else if character == LUMBERYARD_CHARACTER {
            return Cell {
                soil: SoilType::Lumberyard,
            };
        }
        panic!("Invalid character.")
    }

    fn to_char(&self) -> char {
        return self.soil.to_char();
    }
}

#[derive(Clone, PartialOrd, PartialEq)]
struct Grid {
    matrix: Vec<Vec<Cell>>,
}

impl Grid {
    fn count(&self, soil: SoilType) -> u64 {
        let mut counter = 0;
        for row in self.matrix.iter() {
            for cell in row.iter() {
                if cell.soil == soil {
                    counter += 1;
                }
            }
        }
        return counter;
    }

    fn score(&self) -> u64 {
        let trees = self.count(SoilType::Trees);
        let yards = self.count(SoilType::Lumberyard);
        return trees * yards;
    }

    fn count_adjacent(&self, i: usize, j: usize, soil: SoilType) -> u32 {
        let mut counter = 0;
        for si in (i as i32) - 1..(i as i32) + 2 {
            for sj in (j as i32) - 1..(j as i32) + 2 {
                if si >= 0 && si < self.matrix.len() as i32 {
                    if sj >= 0 && sj < self.matrix[si as usize].len() as i32 {
                        if (si as usize, sj as usize) != (i, j) {
                            if self.matrix[si as usize][sj as usize].soil == soil {
                                counter += 1;
                            }
                        }
                    }
                }
            }
        }
        return counter;
    }

    fn update(&mut self) {
        let mut updated_matrix = self.matrix.clone();
        for (i, row) in self.matrix.iter().enumerate() {
            for (j, cell) in row.iter().enumerate() {
                let trees = self.count_adjacent(i, j, SoilType::Trees);
                let yards = self.count_adjacent(i, j, SoilType::Lumberyard);
                match cell.soil {
                    SoilType::OpenGround => {
                        if trees >= 3 {
                            updated_matrix[i][j].soil = SoilType::Trees;
                        }
                    }
                    SoilType::Trees => {
                        if yards >= 3 {
                            updated_matrix[i][j].soil = SoilType::Lumberyard;
                        }
                    }
                    SoilType::Lumberyard => {
                        if !(trees >= 1 && yards >= 1) {
                            updated_matrix[i][j].soil = SoilType::OpenGround;
                        }
                    }
                }
            }
        }
        self.matrix = updated_matrix;
    }

    fn print(&self) {
        for row in self.matrix.iter() {
            for cell in row.iter() {
                print!("{}", cell.to_char());
            }
            println!();
        }
    }
}

fn make_grid(filename: &str) -> Grid {
    let lines = advent::read_lines_from_file(filename);
    let mut matrix = Vec::new();
    for line in lines.iter() {
        matrix.push(Vec::new());
        for character in line.chars() {
            matrix.last_mut().unwrap().push(Cell::from_char(character));
        }
    }
    return Grid { matrix };
}

fn solve_for_iterations(filename: &str, iterations: u64) -> u64 {
    let mut grid = make_grid(filename);
    let mut grids = Vec::new();
    grids.push(grid.clone());
    for iteration in 1..iterations + 1 {
        grid.update();
        for (stored_iteration, stored_grid) in grids.iter().enumerate() {
            if &grid == stored_grid {
                let casted_stored_iteration = stored_iteration as u64;
                let cycle_length = iteration - casted_stored_iteration;
                let offset = iterations - casted_stored_iteration;
                let index = (casted_stored_iteration + (offset) % cycle_length) as usize;
                return grids[index].score();
            }
        }
        grids.push(grid.clone());
    }
    return grids.last().unwrap().score();
}

fn solve(filename: &str) {
    println!("{}", solve_for_iterations(filename, 10));
    println!("{}", solve_for_iterations(filename, 1000000000));
}

fn main() {
    solve(&advent::derive_filename(18));
}
