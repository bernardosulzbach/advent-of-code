package org.mafagafogigante.advent

internal class IntRoot(val value: Int, val free: Boolean = false) : Comparable<IntRoot> {
    override fun compareTo(other: IntRoot): Int {
        if (free) {
            if (other.free) {
                return 0
            }
            return -1
        }
        if (other.free) {
            return 1
        }
        return value.compareTo(other.value)
    }

    companion object {
        fun getLowestCommonValueAtLeast(intRoots: List<IntRoot>, minimum: Int): Int? {
            var commonValue: Int? = null
            intRoots
                    .asSequence()
                    .filterNot { it.free }
                    .forEach {
                        if (commonValue == null && it.value >= minimum) {
                            commonValue = it.value
                        } else {
                            if (commonValue != it.value) {
                                return null
                            }
                        }
                    }
            if (commonValue == null) {
                return 0
            }
            return commonValue
        }
    }

}