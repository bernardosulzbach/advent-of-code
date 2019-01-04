extern crate advent;

use std::vec::Vec;

const PART_TWO_MULTIPLIER: u64 = 100;

struct Game {
    marbles: advent::Ring<u64>,
    next_marble_value: u64,
    players: usize,
    current_player: usize,
    scores: Vec<u64>,
}

impl Game {
    fn new(players: usize, marbles: usize) -> Game {
        let mut game = Game {
            marbles: advent::Ring::new(marbles),
            next_marble_value: 1,
            players,
            current_player: 0,
            scores: vec![0; players],
        };
        game.marbles.insert(0);
        return game;
    }

    fn step(&mut self) {
        if self.next_marble_value % 23 == 0 {
            self.scores[self.current_player] += self.next_marble_value;
            for _ in 0..7 {
                self.marbles.go_back();
            }
            self.scores[self.current_player] += self.marbles.remove();
        } else {
            self.marbles.advance();
            self.marbles.insert(self.next_marble_value);
        }
        self.current_player = (self.current_player + 1) % self.players;
        self.next_marble_value += 1;
    }
}

fn solve_specific(players: u32, marbles: u64) {
    let mut game = Game::new(players as usize, marbles as usize);
    while game.next_marble_value <= marbles {
        game.step();
    }
    println!("{}", game.scores.iter().max().unwrap());
}

fn solve(filename: &str) {
    let lines = advent::read_lines_from_file(filename);
    for line in lines {
        let players_string = line.split_whitespace().nth(0).unwrap();
        let players = players_string.parse::<u32>().unwrap();
        let marbles_string = line.split_whitespace().nth(6).unwrap();
        let marbles = marbles_string.parse::<u64>().unwrap();
        solve_specific(players, marbles);
        solve_specific(players, PART_TWO_MULTIPLIER * marbles);
    }
}

fn main() {
    solve(&advent::derive_filename(9));
}
