package com.bernardosulzbach.advent

import java.lang.Math.abs
import java.lang.Math.max

class Solution201711 {

    private fun evaluateDistance(counter: HashMap<String, Int>): Int {
        val n = counter.getOrDefault("n", 0)
        val nw = counter.getOrDefault("nw", 0)
        val ne = counter.getOrDefault("ne", 0)
        val sw = counter.getOrDefault("sw", 0)
        val se = counter.getOrDefault("se", 0)
        val s = counter.getOrDefault("s", 0)
        val x = n + ne - sw - s
        val y = se + s - n - nw
        val z = sw + nw - se - ne
        return (abs(x) + abs(y) + abs(z)) / 2
    }

    fun getFirstSolution(): Int {
        val tokens = javaClass.getResource("/2017-11.txt").readText(Charsets.UTF_8).trim().split(",")
        val counter = HashMap<String, Int>()
        for (token in tokens) {
            counter.put(token, counter.getOrDefault(token, 0) + 1)
        }
        return evaluateDistance(counter)
    }

    fun getSecondSolution(): Int {
        val tokens = javaClass.getResource("/2017-11.txt").readText(Charsets.UTF_8).trim().split(",")
        val counter = HashMap<String, Int>()
        var maxDistance = 0
        for (token in tokens) {
            counter.put(token, counter.getOrDefault(token, 0) + 1)
            maxDistance = max(maxDistance, evaluateDistance(counter))
        }
        return maxDistance
    }

}

fun main(args: Array<String>) {
    val solution = Solution201711()
    println(solution.getFirstSolution())
    println(solution.getSecondSolution())
}