package org.mafagafogigante.advent

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

internal class Solution201719Test {

    @Test
    fun testWalkWorksAsExpected() {
        val maze = "     |          \n     |  +--+    \n     A  |  C    \n F---|----E|--+ \n     |  |  |  D \n     +B-+  +--+ \n"
        Assertions.assertEquals(Pair("ABCDEF", 38), Solution201719().walk(maze))
    }

}