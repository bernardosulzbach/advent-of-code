package com.bernardosulzbach.advent

import java.lang.Math.max

class Solution201708 {

    private fun solve(): Pair<Int, Int> {
        val conditions = ArrayList<Array<String>>()
        val instructions = ArrayList<Array<String>>()
        javaClass.getResource("/2017-08.txt").readText(Charsets.UTF_8).trim().split('\n').forEach({
            val tokens = it.split(' ')
            conditions.add(arrayOf(tokens[4], tokens[5], tokens[6]))
            instructions.add(arrayOf(tokens[0], tokens[1], tokens[2]))
        })
        var allTimeMaximum = 0
        val registers = HashMap<String, Int>()
        for (i in 0..conditions.size - 1) {
            val condition = conditions[i]
            val registerA = condition[0]
            val comparator = condition[1]
            val constant = condition[2].toInt()
            registers.putIfAbsent(registerA, 0)
            val registerAValue = registers[registerA]!!
            var execute = false
            execute = execute || (comparator == "<" && registerAValue < constant)
            execute = execute || (comparator == "<=" && registerAValue <= constant)
            execute = execute || (comparator == "==" && registerAValue == constant)
            execute = execute || (comparator == ">=" && registerAValue >= constant)
            execute = execute || (comparator == ">" && registerAValue > constant)
            execute = execute || (comparator == "!=" && registerAValue != constant)
            if (execute) {
                val instruction = instructions[i]
                val registerB = instruction[0]
                val constantB = instruction[2].toInt()
                registers.putIfAbsent(registerB, 0)
                val result: Int
                if (instruction[1] == "inc") {
                    result = registers[registerB]!! + constantB
                } else {
                    result = registers[registerB]!! - constantB
                }
                allTimeMaximum = max(allTimeMaximum, result)
                registers[registerB] = result
            }
        }
        val endMaximum = registers.values.max()!!
        return Pair(endMaximum, allTimeMaximum)
    }

    fun getFirstSolution(): Int {
        return solve().first
    }

    fun getSecondSolution(): Int {
        return solve().second
    }

}

fun main(args: Array<String>) {
    val solution = Solution201708()
    println(solution.getFirstSolution())
    println(solution.getSecondSolution())
}