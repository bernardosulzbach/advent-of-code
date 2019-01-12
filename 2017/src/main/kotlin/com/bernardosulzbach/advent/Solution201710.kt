package com.bernardosulzbach.advent

import java.util.*
import kotlin.collections.ArrayList

class Solution201710 {

    private fun sliceSwap(values: List<Int>, start: Int, length: Int) {
        var a = start % values.size
        var b = (start + length - 1) % values.size
        for (x in 1..length / 2) {
            Collections.swap(values, a, b)
            a = Math.floorMod(a + 1, values.size)
            b = Math.floorMod(b - 1, values.size)
        }
    }

    private fun condenseHash(values: List<Int>): List<Int> {
        val result = ArrayList<Int>()
        for (i in 0..values.size - 1 step 16) {
            var accumulator = 0
            for (j in 0..15) {
                accumulator = accumulator.xor(values[i + j])
            }
            result.add(accumulator)
        }
        return result
    }

    fun simulate(values: List<Int>, lengths: List<Int>, rounds: Int): List<Int> {
        var start = 0
        var skip = 0
        for (round in 0..rounds - 1) {
            for (length in lengths) {
                sliceSwap(values, start, length)
                start += length + skip
                skip++
            }
        }
        return values
    }

    fun simulate(values: List<Int>, lengths: List<Int>): List<Int> {
        return simulate(values, lengths, 1)
    }

    fun calculateKnotHash(ints: List<Int>): String {
        val suffix = arrayListOf(17, 31, 73, 47, 23)
        val range = (0..255).mapTo(ArrayList<Int>(), { x -> x })
        val result = simulate(range, ints + suffix, 64)
        val condensed = condenseHash(result)
        return condensed.map({ x -> "%02x".format(x) }).joinToString("")
    }

    fun getFirstSolution(): Int {
        val ints = javaClass.getResource("/2017-10.txt").readText(Charsets.UTF_8).trim().split(',').map(String::toInt)
        val range = (0..255).mapTo(ArrayList<Int>(), { x -> x })
        val result = simulate(range, ints)
        return result[0] * result[1]
    }

    fun getSecondSolution(): String {
        val ints = javaClass.getResource("/2017-10.txt").readText(Charsets.UTF_8).trim().map(Char::toInt)
        return calculateKnotHash(ints)
    }

}

fun main(args: Array<String>) {
    val solution = Solution201710()
    println(solution.getFirstSolution())
    println(solution.getSecondSolution())
}
