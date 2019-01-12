package com.bernardosulzbach.advent

import java.util.*

class Solution201716 {

    fun getInstructions() = javaClass.getResource("/2017-16.txt").readText(Charsets.UTF_8).trim().split(',')

    fun execute(data: ArrayList<Char>, instructions: List<String>) {
        val size = 16
        val swap = CharArray(size)
        for (instruction in instructions) {
            val instructionOperands = instruction.slice(1..instruction.length - 1)
            if (instruction[0] == 's') {
                for (i in 0..size - 1) {
                    val offset = size - instructionOperands.toInt()
                    swap[i] = data[(i + offset) % size]
                }
                for (i in 0..size - 1) {
                    data[i] = swap[i]
                }
            } else if (instruction[0] == 'x') {
                val split = instructionOperands.split('/')
                val a = split[0].toInt()
                val b = split[1].toInt()
                Collections.swap(data, a, b)
            } else {
                val split = instructionOperands.split('/')
                val a = split[0][0]
                val b = split[1][0]
                Collections.swap(data, data.indexOf(a), data.indexOf(b))
            }
        }
    }

    fun getPermutation(iterations: Int): String {
        val instructions = getInstructions()
        val size = 16
        val data = (0..size - 1).mapTo(ArrayList<Char>(size)) { 'a' + it }
        val map = HashMap<List<Char>, Int>()
        var cycleLength = 0
        var iterationsLeft = iterations
        while (iterationsLeft > 0) {
            val lastSeen = map.put(data, iterationsLeft)
            if (lastSeen != null) {
                cycleLength = lastSeen - iterationsLeft
            }
            execute(data, instructions)
            iterationsLeft--
            if (cycleLength != 0) {
                iterationsLeft %= cycleLength
            }
        }
        return String(data.toCharArray())
    }

    fun getFirstSolution(): String {
        return getPermutation(1)
    }

    fun getSecondSolution(): String {
        return getPermutation(1000 * 1000 * 1000)
    }

}

fun main(args: Array<String>) {
    val solution = Solution201716()
    println(solution.getFirstSolution())
    println(solution.getSecondSolution())
}
