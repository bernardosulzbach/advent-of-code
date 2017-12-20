package org.mafagafogigante.advent

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

internal class Solution201709Test {

    @Test
    fun testRemoveGarbageShouldRemoveAllGarbage() {
        Assertions.assertEquals("", Solution201709().removeGarbage("<>"))
        Assertions.assertEquals("", Solution201709().removeGarbage("<random characters>"))
        Assertions.assertEquals("", Solution201709().removeGarbage("<<<<>"))
        Assertions.assertEquals("", Solution201709().removeGarbage("<{!>}>"))
        Assertions.assertEquals("", Solution201709().removeGarbage("<!!>"))
        Assertions.assertEquals("", Solution201709().removeGarbage("<{!>}>"))
        Assertions.assertEquals("", Solution201709().removeGarbage("<!!!>>"))
        Assertions.assertEquals("", Solution201709().removeGarbage("<{o\"i!a,<{i<a>"))
    }

    @Test
    fun testRemoveGarbageShouldDetectedEscapedEscapes() {
        Assertions.assertEquals("{{}}", Solution201709().removeGarbage("{<!!>{}<>}"))
    }

    @Test
    fun testBuildGroupShouldBeReversible() {
        val solution = Solution201709()
        val testA = "{{{},{{},{}}}}"
        val testB = "{{{{},{{},{}}}},{{{}}},{{}},{},{{},{},{},{}}}"
        val testC = solution.removeRedundantCommas(solution.removeGarbage(solution.getText()))
        Assertions.assertEquals(testA, solution.buildGroup(testA).toString())
        Assertions.assertEquals(testB, solution.buildGroup(testB).toString())
        Assertions.assertEquals(testC, solution.buildGroup(testC).toString())
    }

    @Test
    fun testGroupGetScoreShouldWork() {
        val solution = Solution201709()
        Assertions.assertEquals(1, solution.buildGroup("{}").getScore())
        Assertions.assertEquals(6, solution.buildGroup("{{{}}}").getScore())
        Assertions.assertEquals(5, solution.buildGroup("{{},{}}").getScore())
        Assertions.assertEquals(16, solution.buildGroup("{{{},{},{{}}}}").getScore())
        Assertions.assertEquals(1, solution.buildGroup("{<a>,<a>,<a>,<a>}").getScore())
        Assertions.assertEquals(9, solution.buildGroup("{{<ab>},{<ab>},{<ab>},{<ab>}}").getScore())
        Assertions.assertEquals(9, solution.buildGroup("{{<!!>},{<!!>},{<!!>},{<!!>}}").getScore())
        Assertions.assertEquals(3, solution.buildGroup("{{<a!>},{<a!>},{<a!>},{<ab>}}").getScore())
    }

    @Test
    fun testGarbageStatisticsShouldWork() {
        val solution = Solution201709()
        Assertions.assertEquals(0, solution.countGarbage("<>"))
        Assertions.assertEquals(17, solution.countGarbage("<random characters>"))
        Assertions.assertEquals(3, solution.countGarbage("<<<<>"))
        Assertions.assertEquals(2, solution.countGarbage("<{!>}>"))
        Assertions.assertEquals(0, solution.countGarbage("<!!>"))
        Assertions.assertEquals(0, solution.countGarbage("<!!!>>"))
        Assertions.assertEquals(10, solution.countGarbage("<{o\"i!a,<{i<a>"))
    }

}
