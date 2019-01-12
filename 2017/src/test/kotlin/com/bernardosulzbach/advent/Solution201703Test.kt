package com.bernardosulzbach.advent

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

internal class Solution201703Test {

    @Test
    fun getSolution() {
        Assertions.assertEquals(Pair(0, 0), Solution201703(1).getFirstSolution())
        Assertions.assertEquals(Pair(1, 0), Solution201703(2).getFirstSolution())
        Assertions.assertEquals(Pair(1, 1), Solution201703(3).getFirstSolution())
        Assertions.assertEquals(Pair(0, 1), Solution201703(4).getFirstSolution())
        Assertions.assertEquals(Pair(-1, 1), Solution201703(5).getFirstSolution())
        Assertions.assertEquals(Pair(-1, 0), Solution201703(6).getFirstSolution())
        Assertions.assertEquals(Pair(2, 1), Solution201703(12).getFirstSolution())
        Assertions.assertEquals(Pair(2, 2), Solution201703(13).getFirstSolution())
        Assertions.assertEquals(Pair(1, 2), Solution201703(14).getFirstSolution())
    }

}