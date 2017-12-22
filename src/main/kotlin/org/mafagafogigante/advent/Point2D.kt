package org.mafagafogigante.advent

class Point2D(val x: Int, val y: Int) {

    fun getNeighbors() = arrayOf(Point2D(x - 1, y), Point2D(x, y - 1), Point2D(x, y + 1), Point2D(x + 1, y))

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other?.javaClass != javaClass) return false
        other as Point2D
        if (x != other.x) return false
        if (y != other.y) return false
        return true
    }

    override fun hashCode(): Int {
        var result = x
        result = 31 * result + y
        return result
    }

    operator fun plus(other: Point2D): Point2D {
        return Point2D(x + other.x, y + other.y)
    }

}