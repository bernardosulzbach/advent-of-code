package org.mafagafogigante.advent

class Particle<T>(val position: Point3D, val velocity: Point3D, val acceleration: Point3D, val data: T) {

    fun walk(steps: Int): Particle<T> {
        var velocity = velocity
        var position = position
        for (x in 0 until steps) {
            velocity += acceleration
            position += velocity
        }
        return Particle(position, velocity, acceleration, data)
    }

    fun stepsBeforeAccelerationOnlyAccelerates(): Int {
        var minimumSteps = 0
        if (acceleration.x != 0 && ((velocity.x > 0 && acceleration.x < 0) || (velocity.x < 0 && acceleration.x > 0))) {
            minimumSteps = maxOf(minimumSteps, (-velocity.x + (acceleration.x - 1)) / acceleration.x)
        }
        if (acceleration.y != 0 && ((velocity.y > 0 && acceleration.y < 0) || (velocity.y < 0 && acceleration.y > 0))) {
            minimumSteps = maxOf(minimumSteps, (-velocity.y + (acceleration.y - 1)) / acceleration.y)
        }
        if (acceleration.z != 0 && ((velocity.z > 0 && acceleration.z < 0) || (velocity.z < 0 && acceleration.z > 0))) {
            minimumSteps = maxOf(minimumSteps, (-velocity.z + (acceleration.z - 1)) / acceleration.z)
        }
        return minimumSteps
    }

    fun getCollisionStep(other: Particle<T>): Int? {
        val accelerationDeltaX = (acceleration.x - other.acceleration.x) / 2.0
        val accelerationDeltaY = (acceleration.y - other.acceleration.y) / 2.0
        val accelerationDeltaZ = (acceleration.z - other.acceleration.z) / 2.0
        val velocityDeltaX = (velocity.x - other.velocity.x).toDouble()
        val velocityDeltaY = (velocity.y - other.velocity.y).toDouble()
        val velocityDeltaZ = (velocity.z - other.velocity.z).toDouble()
        val positionDeltaX = (position.x - other.position.x).toDouble()
        val positionDeltaY = (position.y - other.position.y).toDouble()
        val positionDeltaZ = (position.z - other.position.z).toDouble()
        // TODO: P(t) = P(0) + from the spreadsheet.
        val roots = ArrayList<IntRoot>()
        val xRoots = findRoots(accelerationDeltaX, velocityDeltaX, positionDeltaX)
        if (xRoots.isEmpty()) {
            return null
        }
        roots.addAll(xRoots)
        val yRoots = findRoots(accelerationDeltaY, velocityDeltaY, positionDeltaY)
        if (yRoots.isEmpty()) {
            return null
        }
        roots.addAll(yRoots)
        val zRoots = findRoots(accelerationDeltaZ, velocityDeltaZ, positionDeltaZ)
        if (zRoots.isEmpty()) {
            return null
        }
        roots.addAll(zRoots)
        return IntRoot.getLowestCommonValueAtLeast(roots, 0)
    }

    private fun isZero(x: Double) = Math.abs(x) < 1E-4

    private fun isNonnegativeInteger(x: Double) = isZero(x - Math.round(x).toInt()) && x.toInt() >= 0

    private fun findRoots(a: Double, b: Double, c: Double): List<IntRoot> {
        if (isZero(a)) {
            if (isZero(b)) {
                if (isZero(c)) {
                    return listOf(IntRoot(0, true))
                }
                return emptyList()
            }
            if (isNonnegativeInteger(-c / b)) {
                return listOf(IntRoot((-c / b).toInt()))
            }
            return emptyList()
        }
        val delta = b * b - 4 * a * c
        if (delta < 0.0) {
            return emptyList()
        }
        val roots = ArrayList<IntRoot>()
        val x1 = (-b + Math.sqrt(delta)) / (2.0 * a)
        val x2 = (-b - Math.sqrt(delta)) / (2.0 * a)
        listOf(x1, x2)
                .filter { isNonnegativeInteger(it) }
                .forEach { roots.add(IntRoot(it.toInt())) }
        roots.sort()
        return roots
    }

}

