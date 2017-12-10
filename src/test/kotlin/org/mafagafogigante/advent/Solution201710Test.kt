package org.mafagafogigante.advent

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

internal class Solution201710Test {

    @Test
    fun simulate() {
        Assertions.assertEquals(listOf(3, 4, 2, 1, 0), Solution201710().simulate(listOf(0, 1, 2, 3, 4), listOf(3, 4, 1, 5)))
    }

}