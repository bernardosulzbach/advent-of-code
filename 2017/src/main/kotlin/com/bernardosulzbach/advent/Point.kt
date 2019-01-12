package com.bernardosulzbach.advent

class Point(val x: Int, val y: Int) {

    fun getNeighbors(): Array<Point> = arrayOf(Point(x - 1, y), Point(x, y - 1), Point(x, y + 1), Point(x + 1, y))

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other?.javaClass != javaClass) return false
        other as Point
        if (x != other.x) return false
        if (y != other.y) return false
        return true
    }

    override fun hashCode(): Int {
        var result = x
        result = 31 * result + y
        return result
    }

}