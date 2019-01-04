extern crate advent;

use std::vec::Vec;

const PART_TWO_MULTIPLIER: u32 = 100;

struct Game {
    marbles: Vec<u32>,
    current_marble_index: usize,
    next_marble_value: u32,
    players: usize,
    current_player: usize,
    scores: Vec<u32>,
}

impl Game {
    fn new(players: usize) -> Game {
        return Game {
            marbles: vec![0; 1],
            current_marble_index: 0,
            next_marble_value: 1,
            players,
            current_player: 0,
            scores: vec![0; players],
        };
    }
    fn step(&mut self) {
        if self.next_marble_value % 23 == 0 {
            self.scores[self.current_player] += self.next_marble_value;
            let safe_subtracted_index = self.current_marble_index + 7 * self.marbles.len() - 7;
            self.current_marble_index = safe_subtracted_index % self.marbles.len();
            self.scores[self.current_player] += self.marbles.remove(self.current_marble_index);
        } else {
            let next_index = (self.current_marble_index + 1) % self.marbles.len() + 1;
            self.marbles.insert(next_index, self.next_marble_value);
            self.current_marble_index = next_index;
        }
        self.current_player = (self.current_player + 1) % self.players;
        self.next_marble_value += 1;
    }
}

fn solve_part_one(players: u32, marbles: u32) {
    let mut game = Game::new(players as usize);
    while game.next_marble_value <= marbles {
        game.step();
    }
    println!("{}", game.scores.iter().max().unwrap());
}

fn solve_part_two(players: u32, marbles: u32) {
    let total_marbles = PART_TWO_MULTIPLIER * marbles;
    let mut game = Game::new(players as usize);
    while game.next_marble_value <= total_marbles {
        if game.next_marble_value % marbles == 0 {
            // TODO: analyze all the player scores after each iteration.
            let score = game.scores.iter().max().unwrap();
            println!("{}x = {}", game.next_marble_value / marbles, score);
        }
        game.step();
    }
}

fn solve(filename: &str) {
    let lines = advent::read_lines_from_file(filename);
    for line in lines {
        let players_string = line.split_whitespace().nth(0).unwrap();
        let players = players_string.parse::<u32>().unwrap();
        let marbles_string = line.split_whitespace().nth(6).unwrap();
        let marbles = marbles_string.parse::<u32>().unwrap();
        solve_part_one(players, marbles);
        solve_part_two(players, marbles);
    }
}

fn main() {
    solve(&advent::derive_filename(9));
}
