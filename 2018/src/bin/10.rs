extern crate advent;

use std::vec::Vec;

struct Vector {
    x: i32,
    y: i32,
}

struct Particle {
    position: Vector,
    velocity: Vector,
}

impl Particle {
    fn from_line(line: &str) -> Particle {
        let (p_slice, v_slice) = line.split_at(line.find(">").unwrap() + 1);
        let p_pair = &p_slice[p_slice.find("<").unwrap() + 1..p_slice.find(">").unwrap()];
        let v_pair = &v_slice[v_slice.find("<").unwrap() + 1..v_slice.find(">").unwrap()];
        let (px, py) = p_pair.split_at(p_pair.find(",").unwrap());
        let (vx, vy) = v_pair.split_at(v_pair.find(",").unwrap());
        let parsed_px = px.trim().parse::<i32>().unwrap();
        let parsed_vx = vx.trim().parse::<i32>().unwrap();
        let parsed_py = py[1..].trim().parse::<i32>().unwrap();
        let parsed_vy = vy[1..].trim().parse::<i32>().unwrap();
        return Particle {
            position: Vector {
                x: parsed_px,
                y: parsed_py,
            },
            velocity: Vector {
                x: parsed_vx,
                y: parsed_vy,
            },
        };
    }
    fn at(&self, time: i32) -> Vector {
        return Vector {
            x: self.position.x + self.velocity.x * time,
            y: self.position.y + self.velocity.y * time,
        };
    }
}

fn evaluate_y_range(particles: &Vec<Particle>, t: i32) -> i32 {
    let mut min_y = std::i32::MAX;
    let mut max_y = std::i32::MIN;
    for particle in particles.iter() {
        let particle_at_t = particle.at(t);
        min_y = std::cmp::min(min_y, particle_at_t.y);
        max_y = std::cmp::max(max_y, particle_at_t.y);
    }
    return max_y - min_y + 1;
}

fn minimize_y_range(particles: &Vec<Particle>) -> i32 {
    let mut y_range = std::i32::MAX;
    let mut t = 0;
    loop {
        let alternative = evaluate_y_range(&particles, t + 1);
        if alternative >= y_range {
            break;
        }
        y_range = alternative;
        t += 1;
    }
    return t;
}

fn solve(filename: &str) {
    let mut particles = Vec::new();
    for line in advent::read_lines_from_file(filename) {
        particles.push(Particle::from_line(&line));
    }
    let t = minimize_y_range(&particles);
    let mut min_x = std::i32::MAX;
    let mut max_x = std::i32::MIN;
    let mut min_y = std::i32::MAX;
    let mut max_y = std::i32::MIN;
    for particle in particles.iter() {
        let particle_at_t = particle.at(t);
        min_x = std::cmp::min(min_x, particle_at_t.x);
        max_x = std::cmp::max(max_x, particle_at_t.x);
        min_y = std::cmp::min(min_y, particle_at_t.y);
        max_y = std::cmp::max(max_y, particle_at_t.y);
    }
    let y_range = max_y - min_y + 1;
    let x_range = max_x - min_x + 1;
    let mut screen = Vec::new();
    for _ in 0..x_range {
        screen.push(vec![' '; y_range as usize]);
    }
    for particle in particles.iter() {
        let particle_at_t = particle.at(t);
        screen[(particle_at_t.x - min_x) as usize][(particle_at_t.y - min_y) as usize] = '#';
    }
    for y in 0..y_range as usize {
        for x in 0..x_range as usize {
            print!("{}", screen[x][y]);
        }
        println!();
    }
    println!("{}", t);
}

fn main() {
    solve(&advent::derive_filename(10));
}
