# Advent of Code solutions

My solutions for the [Advent of Code](https://adventofcode.com) (AoC) events.

One of my goals is to use a different programming language each year.

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

Also, I think that the code I write is of good quality and can help someone who is completely lost to solve a problem.

It might also be interesting to compare solutions written in different languages.
