package org.mafagafogigante.advent

class Solution201719 {

    fun getInput() = javaClass.getResource("/2017-19.txt").readText(Charsets.UTF_8)

    fun walk(input: String): Pair<String, Int> {
        val grid = input.split('\n').filter(String::isNotEmpty)
        var i = 0
        var j = grid.first().indexOf('|')
        var di = 1
        var dj = 0
        val collected = StringBuilder()
        var steps = 0
        while (true) {
            i += di
            j += dj
            steps++
            if (i < 0 || i >= grid.size || j < 0 || j >= grid.first().length || grid[i][j] == ' ') {
                break
            }
            val character = grid[i][j]
            if (character == '+') {
                if (dj == 0) {
                    di = 0
                    if (j - 1 >= 0 && grid[i][j - 1] != ' ') {
                        dj = -1
                    } else {
                        dj = +1
                    }
                } else if (di == 0) {
                    dj = 0
                    if (i - 1 >= 0 && grid[i - 1][j] != ' ') {
                        di = -1
                    } else {
                        di = +1
                    }
                }
            } else if (character == '-' || character == '|') {
            } else {
                collected.append(character)
            }
        }
        return Pair(collected.toString(), steps)
    }

    fun getFirstSolution(): String {
        return walk(getInput()).first
    }

    fun getSecondSolution(): Int {
        return walk(getInput()).second
    }

}

fun main(args: Array<String>) {
    val solution = Solution201719()
    println(solution.getFirstSolution())
    println(solution.getSecondSolution())
}
