# Advent of Code solutions

My solutions for the [Advent of Code](https://adventofcode.com) (AoC) events.

One of my goals is to use a different programming language each year.

## 2020

Using C++ (again).

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## 2019

Using C++.

```bash
mkdir build
cd build
cmake ..
cmake --build .
./runner
```

### Common C++ code

Tests for the common C++ code can be ran the following way.

```bash
mkdir build
cd build
cmake ..
cmake --build .
./tests
```

### C++ style guidelines

Unfortunately, these guidelines are currently often violated.
This is because they reflect what I consider to be good C++ today, not when I wrote most of the solutions.
Some of these rules are not objective, and exist only to ensure consistency.

* The main function should have the signature `void AoC::main(std::istream& stream, U32 const part)`. Include `Main.inl` at the end of the file.
* Define only what you need.
* Use `assert` for expressions that can be removed from the program. Use `AoC::verify` for expressions that can't.
* Hashes should use `AoC::Hash`.
* To test if a value is one of a set of values, use `AoC::isAnyOf(x, y, z, ...)` rather than `x == y || x == z || ...`.
* To convert a signed integer to an unsigned one, use `AoC::unsignedCast<Checked>` or `AoC::unsignedCast<Unchecked>`. If `Checked`, the conversion will be checked at runtime.

## 2018

Using Rust.

```
cargo fmt
cargo build
./target/debug/01
```

### Challenge 5

Linked lists in Rust are a pain.

`std::collections::LinkedList` is quite poor on features.

### Challenge 9

Needed (at least I think so) a circular doubly-linked list.

Ended up allocating a chunk of memory and used indexes into it as pointers.

## 2017

Using Kotlin.

## 2016

Using Python.

```
python3 01.py
```

The lack of typing when using Python makes it really annoying at times.
Simple semantic mistakes which could be caught by the compiler are only detected during execution.
The origin of these mistakes are sometimes hard to track down.

I had to use the `nonlocal` keyword to reference **some** (and only some) variables declared in an outer scope.
There is [a relevant Stack Overflow answer](https://stackoverflow.com/a/5219055/3271844) for an explanation of why this is so.

## Using these solutions

You are free to use my solutions.

I am publishing my solutions because people will eventually publish solutions for these problems anyway.

Additionally, I think that the code I write is of good quality and can help someone who is completely lost to not only solve a problem, but to understand the solution.

It might also be interesting to compare solutions written in different languages.

If you want to point out a problem or fix something yourself, feel free to open a PR to this repository.
