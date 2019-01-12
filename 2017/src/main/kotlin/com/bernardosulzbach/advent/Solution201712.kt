package com.bernardosulzbach.advent

import java.util.*

class Solution201712 {

    private fun getGroupArray(): IntArray {
        val lines = javaClass.getResource("/2017-12.txt").readText(Charsets.UTF_8).trim().split("\n")
        val adjacent = Array(lines.size, { ArrayList<Int>() })
        for (line in lines) {
            val source = line.split("<->")[0].trim().toInt()
            for (target in line.split("<->")[1].trim().replace(" ", "").split(",").map(String::toInt)) {
                adjacent[source].add(target)
            }
        }
        val group = IntArray(lines.size, { 0 })
        var nextGroup = 1
        val seen = HashSet<Int>()
        for (i in 0..lines.size - 1) {
            if (group[i] != 0) {
                continue
            }
            group[i] = nextGroup++
            val queue = LinkedList<Int>()
            queue.add(i)
            while (queue.isNotEmpty()) {
                val head = queue.poll()
                group[head] = group[i]
                seen.add(head)
                adjacent[head].filterNotTo(queue) { seen.contains(it) }
            }
        }
        return group
    }

    fun getFirstSolution(): Int {
        val groupArray = getGroupArray()
        return groupArray.count { x -> x == groupArray[0] }
    }

    fun getSecondSolution() = getGroupArray().toSet().size

}

fun main(args: Array<String>) {
    val solution = Solution201712()
    println(solution.getFirstSolution())
    println(solution.getSecondSolution())
}