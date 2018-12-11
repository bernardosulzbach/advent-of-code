extern crate advent;

use std::vec::Vec;

// Make a bounding box for the known places.
// If a place can exit the bounding box, then it can go on forever in that direction.
// If a place is contained, it will be contained within the bounding box.

struct Point {
    x: i32,
    y: i32,
}

impl Point {
    fn distance(&self, point: &Point) -> u32 {
        let dx = (std::cmp::max(point.x, self.x) - std::cmp::min(point.x, self.x)) as u32;
        let dy = (std::cmp::max(point.y, self.y) - std::cmp::min(point.y, self.y)) as u32;
        return dx + dy;
    }
}

struct Place {
    id: u16,
    point: Point,
}

const NO_PLACE: u16 = std::u16::MAX;
const MAXIMUM_COMBINED_DISTANCE: u32 = 10000;

struct BoundingBox {
    min_x: i32,
    min_y: i32,
    max_x: i32,
    max_y: i32,
    nearest_place: Vec<Vec<u16>>,
}

impl BoundingBox {
    fn new(places: &Vec<Place>) -> BoundingBox {
        let mut bounding_box = BoundingBox {
            min_x: 0,
            min_y: 0,
            max_x: 0,
            max_y: 0,
            nearest_place: Vec::new(),
        };
        for place in places.iter() {
            bounding_box.min_x = std::cmp::min(bounding_box.min_x, place.point.x);
            bounding_box.min_y = std::cmp::min(bounding_box.min_y, place.point.y);
            bounding_box.max_x = std::cmp::max(bounding_box.max_x, place.point.x);
            bounding_box.max_y = std::cmp::max(bounding_box.max_y, place.point.y);
        }
        for x in bounding_box.min_x..bounding_box.max_x + 1 {
            bounding_box.nearest_place.push(Vec::new());
            for _ in bounding_box.min_y..bounding_box.max_y + 1 {
                bounding_box.nearest_place[(x - bounding_box.min_x) as usize].push(NO_PLACE);
            }
        }
        for x in bounding_box.min_x..bounding_box.max_x + 1 {
            for y in bounding_box.min_y..bounding_box.max_y + 1 {
                let mut minimum_distance = std::u32::MAX;
                let mut minimum_distance_place_id = NO_PLACE;
                for place in places.iter() {
                    let distance_from_point = place.point.distance(&Point { x, y });
                    if distance_from_point < minimum_distance {
                        minimum_distance = distance_from_point;
                        minimum_distance_place_id = place.id;
                    } else if distance_from_point == minimum_distance {
                        minimum_distance_place_id = NO_PLACE;
                    }
                }
                let i = (x - bounding_box.min_x) as usize;
                let j = (y - bounding_box.min_y) as usize;
                bounding_box.nearest_place[i][j] = minimum_distance_place_id;
            }
        }
        return bounding_box;
    }
    fn count_points_within_maximum_combined_distance(&self, places: &Vec<Place>) -> u32 {
        let mut count = 0;
        for x in self.min_x..self.max_x + 1 {
            for y in self.min_y..self.max_y + 1 {
                let mut combined_distance = 0u32;
                for place in places.iter() {
                    combined_distance += place.point.distance(&Point { x, y });
                }
                if combined_distance < MAXIMUM_COMBINED_DISTANCE {
                    count += 1;
                }
            }
        }
        return count;
    }
}

fn find_area(place: &Place, bounding_box: &BoundingBox) -> Option<i32> {
    // If the place is the closest to a point at the edge of the bounding box, it has infinite area.
    for x in bounding_box.min_x..bounding_box.max_x + 1 {
        let delta_x = (x - bounding_box.min_x) as usize;
        let min_y = 0;
        let max_y = (bounding_box.max_y - bounding_box.min_y) as usize;
        if bounding_box.nearest_place[delta_x][min_y] == place.id {
            return None;
        }
        if bounding_box.nearest_place[delta_x][max_y] == place.id {
            return None;
        }
    }
    for y in bounding_box.min_y..bounding_box.max_y + 1 {
        let delta_y = (y - bounding_box.min_y) as usize;
        let min_x = 0;
        let max_x = (bounding_box.max_x - bounding_box.min_x) as usize;
        if bounding_box.nearest_place[min_x][delta_y] == place.id {
            return None;
        }
        if bounding_box.nearest_place[max_x][delta_y] == place.id {
            return None;
        }
    }
    let mut area = 0;
    for x in bounding_box.min_x..bounding_box.max_x + 1 {
        for y in bounding_box.min_y..bounding_box.max_y + 1 {
            let i = (x - bounding_box.min_x) as usize;
            let j = (y - bounding_box.min_y) as usize;
            if bounding_box.nearest_place[i][j] == place.id {
                area += 1;
            }
        }
    }
    return Some(area);
}

fn solve(filename: &str) {
    let lines = advent::read_lines_from_file(filename);
    let mut places = Vec::new();
    for (i, line) in lines.iter().enumerate() {
        let line_number = (i + 1) as u16;
        let (x_string, y_string) = line.split_at(line.find(", ").unwrap());
        let x = x_string.parse::<i32>().expect("Should be an integer.");
        let y = y_string[2..].parse::<i32>().expect("Should be an integer.");
        places.push(Place {
            id: line_number,
            point: Point { x, y },
        })
    }
    let bounding_box = BoundingBox::new(&places);
    let mut best_area = None;
    for place in places.iter() {
        let found_area = find_area(&place, &bounding_box);
        if found_area.is_some() {
            if best_area.is_none() || best_area.unwrap() < found_area.unwrap() {
                best_area = found_area;
            }
        }
    }
    println!("{}", best_area.unwrap());
    let answer = bounding_box.count_points_within_maximum_combined_distance(&places);
    println!("{}", answer);
}

fn main() {
    solve(&advent::derive_filename(6));
}
