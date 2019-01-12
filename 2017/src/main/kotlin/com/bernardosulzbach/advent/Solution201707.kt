package com.bernardosulzbach.advent

import java.util.*

internal class Graph(val nameToIndex: HashMap<String, Int>,
                     val indexToName: HashMap<Int, String>,
                     val adjacent: Array<ArrayList<Int>>,
                     val inDegree: IntArray,
                     val value: IntArray)

class Solution201707 {

    private fun buildGraph(): Graph {
        val lines = javaClass.getResource("/2017-07.txt").readText(Charsets.UTF_8).trim().split("\n")
        val nameToIndex = HashMap<String, Int>()
        val indexToName = HashMap<Int, String>()
        val adjacent = Array<ArrayList<Int>>(lines.size, { ArrayList() })
        val inDegree = IntArray(lines.size)
        val value = IntArray(lines.size)
        var nextIndex = 0
        for (line in lines) {
            val tokens = line.filter { c -> c != ',' }.split(" ")
            val programName = tokens[0]
            val programValue = tokens[1].slice(1..tokens[1].length - 2).toInt()
            var index: Int
            if (!nameToIndex.containsKey(programName)) {
                index = nextIndex
                nextIndex++
            } else {
                index = nameToIndex.getOrDefault(programName, 0)
            }
            nameToIndex.put(programName, index)
            indexToName.put(index, programName)
            value[index] = programValue
            for (i in 3..tokens.size - 1) {
                val adjacentName = tokens[i]
                var adjacentIndex: Int
                if (nameToIndex.containsKey(adjacentName)) {
                    adjacentIndex = nameToIndex.getOrDefault(adjacentName, 0)
                } else {
                    adjacentIndex = nextIndex
                    nextIndex++
                    nameToIndex.put(adjacentName, adjacentIndex)
                    indexToName.put(adjacentIndex, adjacentName)
                }
                adjacent[index].add(adjacentIndex)
                inDegree[adjacentIndex]++
            }
        }
        return Graph(nameToIndex, indexToName, adjacent, inDegree, value)
    }

    fun getFirstSolution(): String {
        val graph = buildGraph()
        var root = Int.MIN_VALUE
        (0..graph.inDegree.size - 1)
                .asSequence()
                .filter { graph.inDegree[it] == 0 }
                .forEach {
                    if (root == Int.MIN_VALUE) {
                        root = it
                    } else {
                        throw IllegalStateException()
                    }
                }
        return graph.indexToName.getOrDefault(root, "")
    }

    fun getSecondSolution(): Int {
        val graph = buildGraph()
        val rootName = getFirstSolution()
        val rootIndex = graph.nameToIndex.getOrDefault(rootName, 0)
        fun computeWeight(index: Int): Int {
            val weightFrequency = HashMap<Int, Int>()
            var weight = graph.value[index]
            val lastIndexToWeight = HashMap<Int, Int>()
            for (adjacent in graph.adjacent[index]) {
                val adjacentWeight = computeWeight(adjacent)
                weight += adjacentWeight
                weightFrequency.put(adjacentWeight, weightFrequency.getOrDefault(adjacentWeight, 0) + 1)
                lastIndexToWeight.put(adjacentWeight, adjacent)
            }
            if (weightFrequency.size < 2) {
                return weight
            }
            for ((nodeWeight, nodeWeightFrequency) in weightFrequency) {
                if (nodeWeightFrequency == 1) {
                    val currentWeight = graph.value[lastIndexToWeight[nodeWeight]!!]
                    for ((otherNodeWeight, _) in weightFrequency) {
                        if (nodeWeight != otherNodeWeight) {
                            throw UnbalancedException(currentWeight + (otherNodeWeight - nodeWeight))
                        }
                    }
                }
            }
            throw AssertionError()
        }

        var answer = -1
        try {
            computeWeight(rootIndex)
        } catch (exception: UnbalancedException) {
            answer = exception.value
        }
        return answer
    }

}

class UnbalancedException(val value: Int) : Throwable()

fun main(args: Array<String>) {
    println(Solution201707().getFirstSolution())
    println(Solution201707().getSecondSolution())
}