extern crate advent;

use std::collections::HashMap;
use std::collections::HashSet;
use std::vec::Vec;

type Register = usize;
type Value = u64;

const INSTRUCTION_TYPE_COUNT: u16 = 16;

enum Ignored {
    Ignored,
}

enum Instruction {
    AddRegister(Register, Register, Register),
    AddImmediate(Register, Value, Register),
    MultiplyRegister(Register, Register, Register),
    MultiplyImmediate(Register, Value, Register),
    BitwiseAndRegister(Register, Register, Register),
    BitwiseAndImmediate(Register, Value, Register),
    BitwiseOrRegister(Register, Register, Register),
    BitwiseOrImmediate(Register, Value, Register),
    SetRegister(Register, Ignored, Register),
    SetImmediate(Value, Ignored, Register),
    GreaterThanImmediateRegister(Value, Register, Register),
    GreaterThanRegisterImmediate(Register, Value, Register),
    GreaterThanRegisterRegister(Register, Register, Register),
    EqualImmediateRegister(Value, Register, Register),
    EqualRegisterImmediate(Register, Value, Register),
    EqualRegisterRegister(Register, Register, Register),
}

impl Instruction {
    fn get_code(&self) -> u16 {
        match self {
            Instruction::AddRegister(_, _, _) => 0,
            Instruction::AddImmediate(_, _, _) => 1,
            Instruction::MultiplyRegister(_, _, _) => 2,
            Instruction::MultiplyImmediate(_, _, _) => 3,
            Instruction::BitwiseAndRegister(_, _, _) => 4,
            Instruction::BitwiseAndImmediate(_, _, _) => 5,
            Instruction::BitwiseOrRegister(_, _, _) => 6,
            Instruction::BitwiseOrImmediate(_, _, _) => 7,
            Instruction::SetRegister(_, _, _) => 8,
            Instruction::SetImmediate(_, _, _) => 9,
            Instruction::GreaterThanImmediateRegister(_, _, _) => 10,
            Instruction::GreaterThanRegisterImmediate(_, _, _) => 11,
            Instruction::GreaterThanRegisterRegister(_, _, _) => 12,
            Instruction::EqualImmediateRegister(_, _, _) => 13,
            Instruction::EqualRegisterImmediate(_, _, _) => 14,
            Instruction::EqualRegisterRegister(_, _, _) => 15,
        }
    }
}

fn make_instruction(code: u16, a: u64, b: u64, c: u64) -> Instruction {
    let ra = a as Register;
    let rb = b as Register;
    let rc = c as Register;
    let va = a as Value;
    let vb = b as Value;
    match code {
        0 => Instruction::AddRegister(ra, rb, rc),
        1 => Instruction::AddImmediate(ra, vb, rc),
        2 => Instruction::MultiplyRegister(ra, rb, rc),
        3 => Instruction::MultiplyImmediate(ra, vb, rc),
        4 => Instruction::BitwiseAndRegister(ra, rb, rc),
        5 => Instruction::BitwiseAndImmediate(ra, vb, rc),
        6 => Instruction::BitwiseOrRegister(ra, rb, rc),
        7 => Instruction::BitwiseOrImmediate(ra, vb, rc),
        8 => Instruction::SetRegister(ra, Ignored::Ignored, rc),
        9 => Instruction::SetImmediate(va, Ignored::Ignored, rc),
        10 => Instruction::GreaterThanImmediateRegister(va, rb, rc),
        11 => Instruction::GreaterThanRegisterImmediate(ra, vb, rc),
        12 => Instruction::GreaterThanRegisterRegister(ra, rb, rc),
        13 => Instruction::EqualImmediateRegister(va, rb, rc),
        14 => Instruction::EqualRegisterImmediate(ra, vb, rc),
        15 => Instruction::EqualRegisterRegister(ra, rb, rc),
        _ => panic!(),
    }
}

fn make_all_instructions(a: u64, b: u64, c: u64) -> Vec<Instruction> {
    let ra = a as Register;
    let rb = b as Register;
    let rc = c as Register;
    let va = a as Value;
    let vb = b as Value;
    return vec![
        Instruction::AddRegister(ra, rb, rc),
        Instruction::AddImmediate(ra, vb, rc),
        Instruction::MultiplyRegister(ra, rb, rc),
        Instruction::MultiplyImmediate(ra, vb, rc),
        Instruction::BitwiseAndRegister(ra, rb, rc),
        Instruction::BitwiseAndImmediate(ra, vb, rc),
        Instruction::BitwiseOrRegister(ra, rb, rc),
        Instruction::BitwiseOrImmediate(ra, vb, rc),
        Instruction::SetRegister(ra, Ignored::Ignored, rc),
        Instruction::SetImmediate(va, Ignored::Ignored, rc),
        Instruction::GreaterThanImmediateRegister(va, rb, rc),
        Instruction::GreaterThanRegisterImmediate(ra, vb, rc),
        Instruction::GreaterThanRegisterRegister(ra, rb, rc),
        Instruction::EqualImmediateRegister(va, rb, rc),
        Instruction::EqualRegisterImmediate(ra, vb, rc),
        Instruction::EqualRegisterRegister(ra, rb, rc),
    ];
}

#[derive(Copy, Clone, PartialEq)]
struct Computer {
    registers: [Value; 4],
    panic: bool,
}

impl Computer {
    fn new() -> Computer {
        Computer {
            registers: [0; 4],
            panic: false,
        }
    }

    fn from_string(string: &str) -> Computer {
        let mut registers = [0; 4];
        for (i, sub) in string.split(", ").enumerate() {
            registers[i] = sub.parse::<Value>().unwrap();
        }
        return Computer {
            registers,
            panic: false,
        };
    }

    fn execute(&mut self, instruction: Instruction) {
        if self.panic {
            return;
        }
        match instruction {
            Instruction::AddRegister(a, b, c) => {
                self.registers[c] = self.registers[a] + self.registers[b];
            }
            Instruction::AddImmediate(a, b, c) => {
                self.registers[c] = self.registers[a] + b;
            }
            Instruction::MultiplyRegister(a, b, c) => {
                self.registers[c] = self.registers[a] * self.registers[b];
            }
            Instruction::MultiplyImmediate(a, b, c) => {
                self.registers[c] = self.registers[a] * b;
            }
            Instruction::BitwiseAndRegister(a, b, c) => {
                self.registers[c] = self.registers[a] & self.registers[b];
            }
            Instruction::BitwiseAndImmediate(a, b, c) => {
                self.registers[c] = self.registers[a] & b;
            }
            Instruction::BitwiseOrRegister(a, b, c) => {
                self.registers[c] = self.registers[a] | self.registers[b];
            }
            Instruction::BitwiseOrImmediate(a, b, c) => {
                self.registers[c] = self.registers[a] | b;
            }
            Instruction::SetRegister(a, _, c) => {
                self.registers[c] = self.registers[a];
            }
            Instruction::SetImmediate(a, _, c) => {
                self.registers[c] = a;
            }
            Instruction::GreaterThanImmediateRegister(a, b, c) => {
                self.registers[c] = (a > self.registers[b]) as Value;
            }
            Instruction::GreaterThanRegisterImmediate(a, b, c) => {
                self.registers[c] = (self.registers[a] > b) as Value;
            }
            Instruction::GreaterThanRegisterRegister(a, b, c) => {
                self.registers[c] = (self.registers[a] > self.registers[b]) as Value;
            }
            Instruction::EqualImmediateRegister(a, b, c) => {
                self.registers[c] = (a == self.registers[b]) as Value;
            }
            Instruction::EqualRegisterImmediate(a, b, c) => {
                self.registers[c] = (self.registers[a] == b) as Value;
            }
            Instruction::EqualRegisterRegister(a, b, c) => {
                self.registers[c] = (self.registers[a] == self.registers[b]) as Value;
            }
        }
    }
}

fn split_lines(lines: &Vec<String>) -> (Vec<String>, Vec<String>) {
    let mut part_one_lines = Vec::new();
    let mut part_two_lines = Vec::new();
    let mut in_part_one = true;
    let mut i = 0;
    while i < lines.len() {
        if lines[i].is_empty() {
            i += 1;
            continue;
        }
        if in_part_one {
            if lines[i].starts_with("Before") {
                part_one_lines.push(lines[i].clone());
                part_one_lines.push(lines[i + 1].clone());
                part_one_lines.push(lines[i + 2].clone());
                i += 2;
            } else {
                in_part_one = false;
            }
        }
        if !in_part_one {
            part_two_lines.push(lines[i].clone());
        }

        i += 1;
    }
    return (part_one_lines, part_two_lines);
}

fn solve(filename: &str) {
    // TODO: split the input into two.
    let lines = advent::read_lines_from_file(filename);
    let (one_lines, two_lines) = split_lines(&lines);
    let mut three_or_more_possibilities = 0;
    let mut cannot_be = HashMap::new();
    for i in 0..one_lines.len() {
        if one_lines[i].starts_with("Before") {
            let before_string = one_lines[i][9..one_lines[i].len() - 1].to_string();
            let before_computer = Computer::from_string(before_string.as_ref());
            let instruction_string = one_lines[i + 1].to_string();
            let parts = instruction_string.split_whitespace().collect::<Vec<&str>>();
            let code = parts[0].parse::<u16>().unwrap();
            let a = parts[1].parse::<u64>().unwrap();
            let b = parts[2].parse::<u64>().unwrap();
            let c = parts[3].parse::<u64>().unwrap();
            let after_string = one_lines[i + 2][9..one_lines[i].len() - 1].to_string();
            let after_computer = Computer::from_string(after_string.as_ref());
            let mut count = 0;
            for instruction in make_all_instructions(a, b, c) {
                let current_code = instruction.get_code();
                if !cannot_be.contains_key(&current_code) {
                    cannot_be.insert(instruction.get_code(), HashSet::new());
                }
                let mut computer = before_computer.clone();
                computer.execute(instruction);
                if computer == after_computer {
                    count += 1;
                } else {
                    cannot_be.get_mut(&current_code).unwrap().insert(code);
                }
            }
            if count >= 3 {
                three_or_more_possibilities += 1;
            }
        }
    }
    println!("{}", three_or_more_possibilities);
    let mut translation = HashMap::new();
    while translation.len() < INSTRUCTION_TYPE_COUNT as usize {
        let mut possible_assignments = Vec::new();
        for (code, impossibilities) in cannot_be.iter() {
            let mut count = 0;
            for option in 0..INSTRUCTION_TYPE_COUNT {
                if !impossibilities.contains(&option) {
                    count += 1;
                }
            }
            if count == 1 {
                for option in 0..INSTRUCTION_TYPE_COUNT {
                    if !impossibilities.contains(&option) {
                        possible_assignments.push((option, *code));
                    }
                }
                break;
            }
        }
        if possible_assignments.is_empty() {
            panic!();
        }
        if possible_assignments.len() > 1 {
            panic!();
        }
        let option = possible_assignments[0].0;
        translation.insert(option, possible_assignments[0].1);
        for code in 0..INSTRUCTION_TYPE_COUNT {
            cannot_be.get_mut(&code).unwrap().insert(option);
        }
    }
    let mut computer = Computer::new();
    for line in two_lines {
        let parts = line.split_whitespace().collect::<Vec<&str>>();
        let original_code = parts[0].parse::<u16>().unwrap();
        let a = parts[1].parse::<u64>().unwrap();
        let b = parts[2].parse::<u64>().unwrap();
        let c = parts[3].parse::<u64>().unwrap();
        let code = *translation.get(&original_code).unwrap();
        let instruction = make_instruction(code, a, b, c);
        computer.execute(instruction);
    }
    println!("{}", computer.registers[0])
}

fn main() {
    solve(&advent::derive_filename(16));
}
