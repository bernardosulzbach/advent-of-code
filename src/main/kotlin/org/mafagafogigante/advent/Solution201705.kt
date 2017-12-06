package org.mafagafogigante.advent

class Solution201705 {

    fun getFirstSolution(): Int {
        val lines = javaClass.getResource("/2017-05.txt").readText(Charsets.UTF_8).trim().split("\n")
        val intList = ArrayList<Int>()
        lines.mapTo(intList, String::toInt)
        var i = 0
        var steps = 0
        while (i < intList.size) {
            val address = intList[i]
            intList[i]++
            i += address
            steps++
        }
        return steps
    }

    fun getSecondSolution(): Int {
        val lines = javaClass.getResource("/2017-05.txt").readText(Charsets.UTF_8).trim().split("\n")
        val intList = ArrayList<Int>()
        lines.mapTo(intList, String::toInt)
        var i = 0
        var steps = 0
        while (i < intList.size) {
            val address = intList[i]
            if (intList[i] >= 3) {
                intList[i]--
            } else {
                intList[i]++
            }
            i += address
            steps++
        }
        return steps
    }

}

fun main(args: Array<String>) {
    val solution = Solution201705()
    val firstSolution = solution.getFirstSolution()
    val secondSolution = solution.getSecondSolution()
    println(firstSolution)
    println(secondSolution)
}