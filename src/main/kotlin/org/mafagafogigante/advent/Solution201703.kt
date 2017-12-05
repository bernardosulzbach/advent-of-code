package org.mafagafogigante.advent

import java.lang.Math.abs
import java.lang.Math.max

class Solution201703(val z: Int) {

    fun getFirstSolution(): Pair<Int, Int> {
        var n = z - 1
        var x = 0
        var y = 0
        var r = 1
        while (n > 0) {
            r += 2
            x++
            n--
            var q = r - 2
            while (n > 0 && q > 0) {
                y++
                n--
                q--
            }
            q = r - 1
            while (n > 0 && q > 0) {
                x--
                n--
                q--
            }
            q = r - 1
            while (n > 0 && q > 0) {
                y--
                n--
                q--
            }
            q = r - 1
            while (n > 0 && q > 0) {
                x++
                n--
                q--
            }
        }
        return Pair(x, y)
    }

    fun getSecondSolution(): Int {
        var x = 0
        var y = 0
        var r = 1
        val sum = HashMap<Pair<Int, Int>, Int>()
        var largest = 0
        fun updateSum(x: Int, y: Int) {
            var total = 0
            for (i in x - 1..x + 1) {
                for (j in y - 1..y + 1) {
                    total += sum.getOrDefault(Pair(i, j), 0)
                }
            }
            sum[Pair(x, y)] = total
            largest = max(largest, total)
        }
        sum.put(Pair(0, 0), 1)
        while (largest <= z) {
            r += 2
            x++
            updateSum(x, y)
            var q = r - 2
            while (largest <= z && q > 0) {
                y++
                updateSum(x, y)
                q--
            }
            q = r - 1
            while (largest <= z && q > 0) {
                x--
                updateSum(x, y)
                q--
            }
            q = r - 1
            while (largest <= z && q > 0) {
                y--
                updateSum(x, y)
                q--
            }
            q = r - 1
            while (largest <= z && q > 0) {
                x++
                updateSum(x, y)
                q--
            }
        }
        return largest
    }

}

fun main(args: Array<String>) {
    val solutionSeventeenThree = Solution201703(368078)
    val firstSolution = solutionSeventeenThree.getFirstSolution()
    val secondSolution = solutionSeventeenThree.getSecondSolution()
    println(abs(firstSolution.first) + abs(firstSolution.second))
    println(secondSolution)
}