package org.mafagafogigante.advent

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

internal class IntRootTest {

    @Test
    fun testGetLowestCommonValueWorksWithThreeFreeRoots() {
        val a = IntRoot(0, true)
        val b = IntRoot(0, true)
        val c = IntRoot(0, true)
        Assertions.assertEquals(0, IntRoot.getLowestCommonValue(listOf(a, b, c)))
    }

    @Test
    fun testGetLowestCommonValueWorksWithTwoFreeRoots() {
        val a = IntRoot(1)
        val b = IntRoot(0, true)
        val c = IntRoot(0, true)
        Assertions.assertEquals(1, IntRoot.getLowestCommonValue(listOf(a, b, c)))
    }

    @Test
    fun testGetLowestCommonValueWorksWithOneFreeRootAndTwoEqualRoots() {
        val a = IntRoot(1)
        val b = IntRoot(1)
        val c = IntRoot(0, true)
        Assertions.assertEquals(1, IntRoot.getLowestCommonValue(listOf(a, b, c)))
    }

    @Test
    fun testGetLowestCommonValueWorksWithOneFreeRootAndTwoDifferentRoots() {
        val a = IntRoot(1)
        val b = IntRoot(2)
        val c = IntRoot(0, true)
        Assertions.assertNull(IntRoot.getLowestCommonValue(listOf(a, b, c)))
    }

    @Test
    fun testGetLowestCommonValueWorksWithThreeEqualRoots() {
        val a = IntRoot(1)
        val b = IntRoot(1)
        val c = IntRoot(1, true)
        Assertions.assertEquals(1, IntRoot.getLowestCommonValue(listOf(a, b, c)))
    }

    @Test
    fun testGetLowestCommonValueWorksWithThreeDifferentRoots() {
        val a = IntRoot(1)
        val b = IntRoot(2)
        val c = IntRoot(3)
        Assertions.assertNull(IntRoot.getLowestCommonValue(listOf(a, b, c)))
    }

}