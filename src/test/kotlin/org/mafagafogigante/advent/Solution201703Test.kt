package org.mafagafogigante.advent

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

class Solution201703Test {

    @Test
    fun getSolution() {
        Assertions.assertEquals(Pair(0, 0), SolutionSeventeenThree(1).getFirstSolution())
        Assertions.assertEquals(Pair(1, 0), SolutionSeventeenThree(2).getFirstSolution())
        Assertions.assertEquals(Pair(1, 1), SolutionSeventeenThree(3).getFirstSolution())
        Assertions.assertEquals(Pair(0, 1), SolutionSeventeenThree(4).getFirstSolution())
        Assertions.assertEquals(Pair(-1, 1), SolutionSeventeenThree(5).getFirstSolution())
        Assertions.assertEquals(Pair(-1, 0), SolutionSeventeenThree(6).getFirstSolution())
        Assertions.assertEquals(Pair(2, 1), SolutionSeventeenThree(12).getFirstSolution())
        Assertions.assertEquals(Pair(2, 2), SolutionSeventeenThree(13).getFirstSolution())
        Assertions.assertEquals(Pair(1, 2), SolutionSeventeenThree(14).getFirstSolution())
    }

}