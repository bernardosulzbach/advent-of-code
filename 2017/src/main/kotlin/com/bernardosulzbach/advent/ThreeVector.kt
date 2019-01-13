package com.bernardosulzbach.advent

class ThreeVector(var x: Int, var y: Int, var z: Int) {

    var invalid = false

    override fun toString(): String {
        return "<$x, $y, $z>"
    }

    fun getNormSquare(): Int {
        return x * x + y * y + z * z
    }

    private fun wouldOverflowWhenAdding(x: Int, y: Int): Boolean {
        return ((y > 0 && x > Int.MAX_VALUE - y) || (y < 0 && x < Int.MIN_VALUE - y))
    }

    operator fun plusAssign(other: ThreeVector) {
        invalid = invalid || wouldOverflowWhenAdding(x, other.x)
        x += other.x
        invalid = invalid || wouldOverflowWhenAdding(y, other.y)
        y += other.y
        invalid = invalid || wouldOverflowWhenAdding(z, other.z)
        z += other.z
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as ThreeVector

        if (x != other.x) return false
        if (y != other.y) return false
        if (z != other.z) return false

        return true
    }

    override fun hashCode(): Int {
        var result = x
        result = 31 * result + y
        result = 31 * result + z
        return result
    }

}
