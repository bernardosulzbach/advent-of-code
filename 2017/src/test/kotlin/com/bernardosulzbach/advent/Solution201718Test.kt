package com.bernardosulzbach.advent

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import java.math.BigInteger

internal class Solution201718Test {

    @Test
    fun getFirstSolutionShouldWorkWithTheSampleInstructions() {
        val sampleInstructions = """
        set a 1
        add a 2
        mul a a
        mod a 5
        snd a
        set a 0
        rcv a
        jgz a -1
        set a 1
        jgz a -2"""
        val solution = Solution201718()
        val instructions = solution.getInstructions(sampleInstructions)
        Assertions.assertEquals(BigInteger("4"), solution.getFirstSolution(instructions))
    }

}