package org.mafagafogigante.advent

import java.lang.Math.max

class Solution201713 {

    fun getFirstSolution(): Int {
        val lines = javaClass.getResource("/2017-13.txt").readText(Charsets.UTF_8).trim().split("\n")
        var severity = 0
        for (line in lines) {
            val depth = line.split(": ")[0].trim().toInt()
            val range = line.split(": ")[1].trim().toInt()
            val period = max(1, 2 * range - 2)
            if (depth % (period) == 0) {
                severity += depth * range
            }
        }
        return severity
    }

    private fun passesWithoutBeingCaught(depths: IntArray, ranges: IntArray, delay: Int): Boolean {
        for ((i, depth) in depths.withIndex()) {
            val range = ranges[i]
            val period = max(1, 2 * range - 2)
            if ((depth + delay) % (period) == 0) {
                return false
            }
        }
        return true
    }

    fun getSecondSolution(): Int {
        val lines = javaClass.getResource("/2017-13.txt").readText(Charsets.UTF_8).trim().split("\n")
        val depths = lines.map({ line -> line.split(": ")[0].trim().toInt() }).toIntArray()
        val ranges = lines.map({ line -> line.split(": ")[1].trim().toInt() }).toIntArray()
        var delay = 0
        while (!passesWithoutBeingCaught(depths, ranges, delay)) {
            delay++
        }
        return delay
    }

}

fun main(args: Array<String>) {
    val solution = Solution201713()
    println(solution.getFirstSolution())
    println(solution.getSecondSolution())
}