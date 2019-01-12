package com.bernardosulzbach.advent

class Solution201701 {

    fun getFirstSolution(): Int {
        val string = javaClass.getResource("/2017-01.txt").readText(Charsets.UTF_8).trim()
        val n = string.length
        val sum = (0..n - 1)
                .filter { string[it] == string[(it + 1) % n] }
                .sumBy { string[it] - '0' }
        return sum
    }

    fun getSecondSolution(): Int {
        val string = javaClass.getResource("/2017-01.txt").readText(Charsets.UTF_8).trim()
        val n = string.length
        val sum = (0..n - 1).filter { string[it] == string[(it + n / 2) % n] }
                .sumBy { string[it] - '0' }
        return sum
    }

}

fun main(args: Array<String>) {
    val solution = Solution201701()
    println(solution.getFirstSolution())
    println(solution.getSecondSolution())
}