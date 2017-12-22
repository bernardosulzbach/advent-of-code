package org.mafagafogigante.advent

class Point3D(val x: Int, val y: Int, val z: Int) {

    companion object {
        val ORIGIN = Point3D(0, 0, 0)
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other?.javaClass != javaClass) return false

        other as Point3D

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

    operator fun plus(other: Point3D): Point3D {
        return Point3D(x + other.x, y + other.y, z + other.z)
    }

    fun evaluateL1Distance(other: Point3D): Int {
        return Math.abs(x - other.x) + Math.abs(y - other.y) + Math.abs(z - other.z)
    }

}
