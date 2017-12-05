package org.mafagafogigante.advent

class Solution201702 {

    fun getFirstSolution(): Int {
        var checksum = 0
        for (line in javaClass.getResource("/2017-02.txt").readText(Charsets.UTF_8).split("\n")) {
            if (line.isEmpty()) {
                continue
            }
            var min = Integer.MAX_VALUE
            var max = Integer.MIN_VALUE
            for (bit in line.split("\t")) {
                min = Math.min(min, bit.toInt())
                max = Math.max(max, bit.toInt())
            }
            checksum += max - min
        }
        return checksum
    }

    fun getSecondSolution(): Int {
        var sum = 0
        for (line in javaClass.getResource("/2017-02.txt").readText(Charsets.UTF_8).split("\n")) {
            if (line.isEmpty()) {
                continue
            }
            val integers = ArrayList<Int>()
            line.split("\t").mapTo(integers, String::toInt)
            for (i in 0..integers.size - 1) {
                for (j in i + 1..integers.size - 1) {
                    if (integers[i] % integers[j] == 0) {
                        sum += integers[i] / integers[j]
                    }
                    if (integers[j] % integers[i] == 0) {
                        sum += integers[j] / integers[i]
                    }
                }
            }
        }
        return sum
    }
}

fun main(args: Array<String>) {
    val solution = Solution201702()
    println(solution.getFirstSolution())
    println(solution.getSecondSolution())
}