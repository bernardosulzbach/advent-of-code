package org.mafagafogigante.advent

class Solution201704 {

    fun getFirstSolution(): Int {
        val lines = javaClass.getResource("/2017-04.txt").readText(Charsets.UTF_8).trim().split("\n")
        var count = 0
        for (line in lines) {
            val set = HashSet<String>()
            var failed = false
            for (token in line.split(" ")) {
                if (set.contains(token)) {
                    failed = true
                    break
                }
                set.add(token)
            }
            if (!failed) {
                count++
            }
        }
        return count
    }

    fun getSecondSolution(): Int {
        val lines = javaClass.getResource("/2017-04.txt").readText(Charsets.UTF_8).trim().split("\n")
        var count = 0
        for (line in lines) {
            val set = HashSet<String>()
            var failed = false
            for (token in line.split(" ")) {
                val charArray = token.toCharArray()
                charArray.sort()
                val sortedToken = String(charArray)
                if (set.contains(sortedToken)) {
                    failed = true
                    break
                }
                set.add(sortedToken)
            }
            if (!failed) {
                count++
            }
        }
        return count
    }

}

fun main(args: Array<String>) {
    val solution = Solution201704()
    val firstSolution = solution.getFirstSolution()
    val secondSolution = solution.getSecondSolution()
    println(firstSolution)
    println(secondSolution)
}