package org.mafagafogigante.advent

import java.util.*

class Solution201714 {

    private fun getInputString() = javaClass.getResource("/2017-14.txt").readText(Charsets.UTF_8).trim()

    private fun populationCount(value: Int): Int {
        var x = value
        var count = 0
        while (x != 0) {
            x = x.and(x - 1)
            count++
        }
        return count
    }

    fun getKnotHash(string: String): String = Solution201710().calculateKnotHash(string.map(Char::toInt))

    fun getKnotHashPopulationCount(string: String): Int {
        var accumulator = 0
        for (character in getKnotHash(string)) {
            val value = Integer.parseInt(character.toString(), 16)
            accumulator += populationCount(value)
        }
        return accumulator
    }

    fun getKnotHashPopulationCountSum(prefix: String, count: Int): Int {
        return (0..count - 1).sumBy { getKnotHashPopulationCount(prefix + "-" + it) }
    }

    fun getFirstSolution() = getKnotHashPopulationCountSum(getInputString(), 128)

    private fun countIsles(grid: Array<IntArray>): Int {
        fun clearIsle(i: Int, j: Int) {
            val seen = HashSet<Point2D>()
            val queue = LinkedList<Point2D>()
            queue.push(Point2D(j, i))
            while (queue.isNotEmpty()) {
                val head = queue.poll()
                if (grid[head.y][head.x] != 0) {
                    grid[head.y][head.x] = 0
                    for (neighbor in head.getNeighbors()) {
                        if (neighbor.y >= 0 && neighbor.y < grid.size && neighbor.x >= 0 && neighbor.x < grid[0].size) {
                            if (grid[neighbor.y][neighbor.x] != 0) {
                                if (!seen.contains(neighbor)) {
                                    seen.add(neighbor)
                                    queue.push(neighbor)
                                }
                            }
                        }
                    }
                }
            }
        }
        var counter = 0
        for (i in 0..grid.size - 1) {
            for (j in 0..grid[i].size - 1) {
                if (grid[i][j] != 0) {
                    counter++
                    clearIsle(i, j)
                }
            }
        }
        return counter
    }

    fun getSecondSolution(): Int {
        val grid = Array(128, { IntArray(128) })
        for (i in 0..127) {
            val hash = getKnotHash(getInputString() + "-" + i)
            for ((j, char) in hash.withIndex()) {
                for (k in 0..3) {
                    grid[i][4 * j + k] = (Integer.parseInt(char.toString(), 16).shr(3 - k)).and(1)
                }
            }
        }
        return countIsles(grid)
    }

}

fun main(args: Array<String>) {
    val solution = Solution201714()
    println(solution.getFirstSolution())
    println(solution.getSecondSolution())
}
