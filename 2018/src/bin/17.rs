extern crate advent;

use std::vec::Vec;

#[derive(Copy, Clone, PartialOrd, PartialEq)]
enum SoilType {
    Sand,
    Clay,
    WetSand,
    Water,
    Source,
}

impl SoilType {
    fn to_char(&self) -> char {
        match self {
            SoilType::Sand => '.',
            SoilType::Clay => '#',
            SoilType::WetSand => '|',
            SoilType::Water => '~',
            SoilType::Source => '+',
        }
    }
}

#[derive(Copy, Clone, PartialOrd, PartialEq)]
enum Flow {
    Open,
    Closed,
    Uninitialized,
}

#[derive(Copy, Clone)]
struct Cell {
    soil: SoilType,
    flow: [Flow; 4],
}

impl Cell {
    fn to_char(&self) -> char {
        return self.soil.to_char();
    }
}

struct Grid {
    matrix: Vec<Vec<Cell>>,
    min_x: i32,
    min_y: i32,
    max_x: i32,
    max_y: i32,
}

impl Grid {
    fn get(&self, x: i32, y: i32) -> Cell {
        return self.matrix[y as usize][(x - self.min_x + 1) as usize].clone();
    }

    fn set_soil(&mut self, x: i32, y: i32, soil: SoilType) {
        self.matrix[y as usize][(x - self.min_x + 1) as usize].soil = soil;
    }

    fn set_flow(&mut self, x: i32, y: i32, direction: advent::Direction, flow: Flow) {
        let y_index = y as usize;
        let x_index = (x - self.min_x + 1) as usize;
        let direction_index = direction.get_index() as usize;
        self.matrix[y_index][x_index].flow[direction_index] = flow;
    }

    fn can_flow(&mut self, x: i32, y: i32, direction: advent::Direction) -> bool {
        if x < self.min_x - 1 || x > self.max_x + 1 || y > self.max_y {
            return true;
        }
        let direction_index = direction.get_index() as usize;
        if self.get(x, y).flow[direction_index] == Flow::Open {
            return true;
        }
        if self.get(x, y).flow[direction_index] == Flow::Closed {
            return false;
        }
        if self.get(x, y).soil == SoilType::Clay {
            return false;
        }
        if direction == advent::Direction::South {
            if self.can_flow(x, y + 1, advent::Direction::South) {
                self.set_soil(x, y, SoilType::WetSand);
            } else {
                let flow_west = self.can_flow(x, y + 1, advent::Direction::West);
                let flow_east = self.can_flow(x, y + 1, advent::Direction::East);
                if flow_west || flow_east {
                    self.set_soil(x, y, SoilType::WetSand);
                } else {
                    self.set_soil(x, y, SoilType::Water);
                }
            }
        } else if direction == advent::Direction::West {
            if self.can_flow(x - 1, y, advent::Direction::South) {
                self.set_soil(x, y, SoilType::WetSand);
            } else if self.can_flow(x - 1, y, advent::Direction::West) {
                self.set_soil(x, y, SoilType::WetSand);
            } else {
                self.set_soil(x, y, SoilType::Water);
            }
        } else if direction == advent::Direction::East {
            if self.can_flow(x + 1, y, advent::Direction::South) {
                self.set_soil(x, y, SoilType::WetSand);
            } else if self.can_flow(x + 1, y, advent::Direction::East) {
                self.set_soil(x, y, SoilType::WetSand);
            } else {
                self.set_soil(x, y, SoilType::Water);
            }
        }
        if self.get(x, y).soil == SoilType::WetSand {
            self.set_flow(x, y, direction, Flow::Open);
            return true;
        }
        if self.get(x, y).soil == SoilType::Water {
            self.set_flow(x, y, direction, Flow::Closed);
            return false;
        }
        panic!("Could not handle this case.");
    }

    fn propagate_wetness(&mut self) {
        for row in self.matrix.iter_mut() {
            let mut preceded_by_wet_sand = false;
            for state in row.iter_mut() {
                if preceded_by_wet_sand && state.soil == SoilType::Water {
                    state.soil = SoilType::WetSand;
                }
                preceded_by_wet_sand = state.soil == SoilType::WetSand;
            }
            preceded_by_wet_sand = false;
            for state in row.iter_mut().rev() {
                if preceded_by_wet_sand && state.soil == SoilType::Water {
                    state.soil = SoilType::WetSand;
                }
                preceded_by_wet_sand = state.soil == SoilType::WetSand;
            }
        }
    }

    fn count_reachable(&self) -> u32 {
        let mut count = 0;
        for (y, row) in self.matrix.iter().enumerate() {
            if y < self.min_y as usize {
                continue;
            }
            for cell in row.iter() {
                if cell.soil == SoilType::WetSand || cell.soil == SoilType::Water {
                    count += 1;
                }
            }
        }
        return count;
    }

    fn count_water(&self) -> u32 {
        let mut count = 0;
        for (y, row) in self.matrix.iter().enumerate() {
            if y < self.min_y as usize {
                continue;
            }
            for cell in row.iter() {
                if cell.soil == SoilType::Water {
                    count += 1;
                }
            }
        }
        return count;
    }

    fn print(&self) {
        for row in self.matrix.iter() {
            for state in row.iter() {
                print!("{}", state.to_char());
            }
            println!();
        }
    }
}

fn make_grid(filename: &str) -> Grid {
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
    let mut matrix = Vec::new();
    for _ in 0..max_y + 1 {
        let mut new_row = Vec::new();
        for _ in min_x..max_x + 3 {
            new_row.push(Cell {
                soil: SoilType::Sand,
                flow: [Flow::Uninitialized; 4],
            });
        }
        matrix.push(new_row);
    }
    for (x_range, y_range) in ranges.iter() {
        for x in x_range.clone() {
            for y in y_range.clone() {
                matrix[y as usize][(x - min_x + 1) as usize].soil = SoilType::Clay;
            }
        }
    }
    matrix[0][(500 - min_x + 1) as usize].soil = SoilType::Source;
    return Grid {
        matrix,
        min_x,
        min_y,
        max_x,
        max_y,
    };
}

fn solve(filename: &str) {
    let mut grid = make_grid(filename);
    grid.can_flow(500, 1, advent::Direction::South);
    grid.propagate_wetness();
    println!("{}", grid.count_reachable());
    println!("{}", grid.count_water());
}

fn main() {
    solve(&advent::derive_filename(17));
}
