package org.mafagafogigante.advent


class Solution201720 {

    fun getInput() = javaClass.getResource("/2017-20.txt").readText(Charsets.UTF_8).trim()

    fun makeParticleList(): List<Particle<Int>> {
        val regex = Regex("<([^>]*)>")
        val particles = ArrayList<Particle<Int>>()
        for ((i, line) in getInput().split('\n').withIndex()) {
            val points = ArrayList<Point3D>()
            for (group in regex.findAll(line).iterator()) {
                val split = group.groups[1]!!.value.split(',')
                points.add(Point3D(split[0].toInt(), split[1].toInt(), split[2].toInt()))
            }
            particles.add(Particle(points[0], points[1], points[2], i))
        }
        return particles
    }

    fun getFirstSolution(): Int {
        val origin = Point3D.ORIGIN
        val allParticles = makeParticleList()
        val slowestAcceleratingAcceleration = allParticles.map { particle ->
            particle.acceleration.evaluateL1Distance(origin)
        }.min()
        val slowestAccelerating = allParticles.filter { particle ->
            particle.acceleration.evaluateL1Distance(origin) == slowestAcceleratingAcceleration
        }
        val stepsRequired = slowestAccelerating.map(Particle<Int>::stepsBeforeAccelerationOnlyAccelerates).max()!!
        val acceleratingForward = slowestAccelerating.map({ it.walk(stepsRequired) })
        val slowestVelocity = acceleratingForward.map { it.velocity.evaluateL1Distance(origin) }.min()
        val slowestParticles = acceleratingForward.filter { it.velocity.evaluateL1Distance(origin) == slowestVelocity }
        return slowestParticles.minBy { particle -> particle.position.evaluateL1Distance(origin) }!!.data
    }

    fun getSecondSolution(): Int {
        val allParticles = makeParticleList()
        val collisions = ArrayList<Collision>()
        for ((i, particleA) in allParticles.withIndex()) {
            for ((j, particleB) in allParticles.withIndex()) {
                if (i < j) {
                    val step = particleA.getCollisionStep(particleB)
                    if (step != null) {
                        collisions.add(Collision(step, i, j))
                    }
                }
            }
        }
        val present = BooleanArray(allParticles.size, { true })
        collisions.sortBy { collision -> collision.step }
        val batch = ArrayList<Collision>()
        var currentStep = 0
        fun processBatch() {
            val removedSet = HashSet<Int>()
            for (collision in batch) {
                if (present[collision.i] && present[collision.j]) {
                    removedSet.add(collision.i)
                    removedSet.add(collision.j)
                }
            }
            for (removed in removedSet) {
                present[removed] = false
            }
        }
        for (collision in collisions) {
            if (collision.step != currentStep) {
                processBatch()
            }
            currentStep = collision.step
            batch.add(collision)
        }
        return present.count { x -> x }
    }

}

internal class Collision(val step: Int, val i: Int, val j: Int)

fun main(args: Array<String>) {
    val solution = Solution201720()
    println(solution.getFirstSolution())
    println(solution.getSecondSolution())
}
