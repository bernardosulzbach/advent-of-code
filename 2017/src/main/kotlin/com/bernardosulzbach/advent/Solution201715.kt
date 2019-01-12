package com.bernardosulzbach.advent

internal class Generator(start: Long, val factor: Long) {

    var accumulated: Long = start

    fun step() {
        accumulated = (accumulated * factor) % 2147483647
    }

}

class Solution201715 {

    private fun getInputValues(): List<Long> {
        val lines = javaClass.getResource("/2017-15.txt").readText(Charsets.UTF_8).trim().split("\n")
        return lines.map({ line -> line.split(" ").last().toLong() })
    }

    fun countMultiples(aStart: Long, bStart: Long, aMultiple: Long, bMultiple: Long, iterations: Int): Int {
        val a = Generator(aStart, 16807)
        val b = Generator(bStart, 48271)
        var counter = 0
        for (i in 0..iterations - 1) {
            a.step()
            while (a.accumulated % aMultiple != 0L) {
                a.step()
            }
            b.step()
            while (b.accumulated % bMultiple != 0L) {
                b.step()
            }
            if (a.accumulated.and(0xFFFF) == b.accumulated.and(0xFFFF)) {
                counter++
            }
        }
        return counter
    }

    fun getFirstSolution(): Int {
        val inputValues = getInputValues()
        return countMultiples(inputValues[0], inputValues[1], 1, 1, 40 * 1000 * 1000)
    }

    fun getSecondSolution(): Int {
        val inputValues = getInputValues()
        return countMultiples(inputValues[0], inputValues[1], 4, 8, 5 * 1000 * 1000)
    }

}

fun main(args: Array<String>) {
    val solution = Solution201715()
    println(solution.getFirstSolution())
    println(solution.getSecondSolution())
}
