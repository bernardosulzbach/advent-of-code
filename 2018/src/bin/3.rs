extern crate advent;

#[derive(Debug, Copy, Clone)]
struct Claim {
    id: u64,
    x: usize,
    y: usize,
    w: usize,
    h: usize,
}

impl Claim {
    fn from_string(string: &str) -> Claim {
        // Sample input: "#1 @ 1,3: 4x4".
        let mut claim = Claim {
            id: 0,
            x: 0,
            y: 0,
            w: 0,
            h: 0,
        };
        for (i, word) in string.to_string().split_whitespace().enumerate() {
            if i == 0 {
                claim.id = word[1..word.len()]
                    .parse()
                    .expect("Failed to parse integer.");
            } else if i == 2 {
                let (x, y) = word[0..word.len() - 1].split_at(word.find(',').unwrap());
                claim.x = x.parse().expect("Failed to parse integer.");
                claim.y = y[1..y.len()].parse().expect("Failed to parse integer.");
            } else if i == 3 {
                let (w, h) = word.split_at(word.find('x').unwrap());
                claim.w = w.parse().expect("Failed to parse integer.");
                claim.h = h[1..h.len()].parse().expect("Failed to parse integer.");
            }
        }
        return claim;
    }
}

fn solve(filename: &str) {
    // There are about 1100 claims.
    // This should make a dense solution using a matrix comparable to a sparse solution.
    // In a sparse solution, we could try to compute the union of all the intersections.
    let mut claims = Vec::new();
    for line in advent::read_lines_from_file(filename) {
        claims.push(Claim::from_string(&line));
    }
    let mut largest_x = 0;
    let mut largest_y = 0;
    for claim in claims.iter() {
        largest_x = std::cmp::max(largest_x, claim.x + claim.w - 1);
        largest_y = std::cmp::max(largest_y, claim.y + claim.h - 1);
    }
    let mut grid = Vec::new();
    for _ in 0..largest_x + 1 {
        grid.push(vec![0; largest_y + 1]);
    }
    for claim in claims.iter() {
        for i in claim.x..claim.x + claim.w {
            for j in claim.y..claim.y + claim.h {
                grid[i][j] = std::cmp::min(2, grid[i][j] + 1);
            }
        }
    }
    let mut count = 0;
    for line in grid.iter() {
        for cell in line.iter() {
            if cell > &1 {
                count += 1;
            }
        }
    }
    println!("{}", count);
    for claim in claims.iter() {
        let mut alone = true;
        for i in claim.x..claim.x + claim.w {
            for j in claim.y..claim.y + claim.h {
                if grid[i][j] > 1 {
                    alone = false;
                }
            }
        }
        if alone {
            println!("{}", claim.id);
        }
    }
}

fn main() {
    let filename = advent::derive_filename(3);
    solve(filename.as_ref());
}
