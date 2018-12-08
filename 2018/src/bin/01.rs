extern crate advent;

fn solve_first_part(filename: &str) {
    let mut total = 0;
    for integer in advent::read_integers_from_file::<i32>(filename) {
        total += integer;
    }
    println!("{}", total);
}

fn solve_second_part(filename: &str) {
    let values = advent::read_integers_from_file::<i32>(filename);
    let mut seen = std::collections::HashSet::new();
    let mut total = 0;
    loop {
        for value in values.iter() {
            total += value;
            if seen.contains(&total) {
                println!("{}", total);
                return;
            }
            seen.insert(total);
        }
    }
}

fn main() {
    let filename = advent::derive_filename(1);
    solve_first_part(filename.as_ref());
    solve_second_part(filename.as_ref());
}
