package com.bernardosulzbach.advent

class Group {

    val children: ArrayList<Group> = ArrayList()

    private fun getScore(base: Int): Int {
        return base + children.map({ child -> child.getScore(base + 1) }).sum()
    }

    fun getScore(): Int {
        return getScore(1)
    }

    override fun toString(): String {
        return "{" + children.map(Group::toString).joinToString(",") + "}"
    }

}

class GarbageStatistics {

    var removed = 0

}

class Solution201709 {

    fun removeGarbage(input: String, statistics: GarbageStatistics): String {
        val size = input.length
        val builder = StringBuilder()
        var isGarbage = false
        fun isNotEscaped(r: Int): Boolean {
            var escaped = false
            (r - 1 downTo 0)
                    .asSequence()
                    .takeWhile { input[it] == '!' }
                    .forEach { escaped = !escaped }
            return !escaped
        }
        for (r in 0..size - 1) {
            if (isGarbage) {
                if (isNotEscaped(r)) {
                    if (input[r] == '>') {
                        isGarbage = false
                    } else {
                        if (input[r] != '!' && isNotEscaped(r)) {
                            statistics.removed++
                        }
                    }
                }
            } else {
                if (input[r] == '<') {
                    isGarbage = true
                } else {
                    builder.append(input[r])
                }
            }
        }
        return builder.toString()
    }

    fun removeGarbage(input: String): String {
        return removeGarbage(input, GarbageStatistics())
    }

    fun countGarbage(input: String): Int {
        val statistics = GarbageStatistics()
        removeGarbage(input, statistics)
        return statistics.removed
    }

    fun removeRedundantCommas(input: String): String {
        val size = input.length
        val builder = StringBuilder()
        for (r in 0..size - 1) {
            if (input[r] == ',') {
                if (input[r - 1] != '}' || input[r + 1] != '{') {
                    continue
                }
            }
            builder.append(input[r])
        }
        return builder.toString()
    }

    private fun sensibleCommaSplit(input: String): List<String> {
        val parts = ArrayList<String>()
        var depth = 0
        var start = 0
        for (i in 0..input.length - 1) {
            if (input[i] == '{') {
                depth++
            } else if (input[i] == '}') {
                depth--
            } else if (depth == 0 && input[i] == ',') {
                parts.add(input.slice(start..i - 1))
                start = i + 1
            }
        }
        parts.add(input.slice(start..input.length - 1))
        return parts
    }

    private fun buildGroupFromCleanString(input: String): Group {
        val group = Group()
        val slice = input.slice(1..input.length - 2)
        if (slice.isNotEmpty()) {
            sensibleCommaSplit(slice).map { s -> group.children.add(buildGroup(s)) }
        }
        return group
    }

    fun buildGroup(input: String) = buildGroupFromCleanString(removeRedundantCommas(removeGarbage(input)))

    fun getText() = javaClass.getResource("/2017-09.txt").readText(Charsets.UTF_8).trim()

    fun getFirstSolution() = buildGroup(getText()).getScore()

    fun getSecondSolution() = countGarbage(getText())

}

fun main(args: Array<String>) {
    val solution = Solution201709()
    println(solution.getFirstSolution())
    println(solution.getSecondSolution())
}