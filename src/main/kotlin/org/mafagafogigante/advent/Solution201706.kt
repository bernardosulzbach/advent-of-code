package org.mafagafogigante.advent

class Solution201706 {

    fun getFirstSolution(): Int {
        val strings = javaClass.getResource("/2017-06.txt").readText(Charsets.UTF_8).trim().split("\t")
        val values = strings.map(String::toInt).toIntArray()
        return solve(values).first
    }

    fun getSecondSolution(): Int {
        val strings = javaClass.getResource("/2017-06.txt").readText(Charsets.UTF_8).trim().split("\t")
        val values = strings.map(String::toInt).toIntArray()
        return solve(values).second
    }

    fun solve(values: IntArray): Pair<Int, Int> {
        var rounds = 0
        val map = HashMap<Int, Int>()
        while (true) {
            val encoded = encode(values)
            if (map.contains(encoded)) {
                break
            }
            map.put(encoded, rounds)
            distribute(values)
            rounds++
        }
        val encoded = encode(values)
        return Pair(rounds, rounds - map.getOrDefault(encoded, 0))
    }

    private fun encode(values: IntArray): Int {
        val primes = intArrayOf(2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53)
        var encoded = 1
        for (i in 0..values.size - 1) {
            for (j in 0..values[i]) {
                encoded *= primes[i]
            }
        }
        return encoded
    }

    private fun distribute(values: IntArray) {
        var i = 0
        (0..values.size - 1)
                .asSequence()
                .filter { values[it] > values[i] }
                .forEach { i = it }
        var x = values[i % values.size]
        values[i] = 0
        i++
        while (x > 0) {
            values[i % values.size]++
            x--
            i++
        }
    }

}

fun main(args: Array<String>) {
    val solution = Solution201706()
    val firstSolution = solution.getFirstSolution()
    val secondSolution = solution.getSecondSolution()
    println(firstSolution)
    println(secondSolution)
}