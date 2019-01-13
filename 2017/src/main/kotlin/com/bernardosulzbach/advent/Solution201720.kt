package com.bernardosulzbach.advent

import java.util.stream.Collectors

class Solution201720 {

    private fun readParticles(): List<Particle> {
        val particles = ArrayList<Particle>()
        for ((index, line) in getInput().split('\n').filter(String::isNotEmpty).withIndex()) {
            val split = line.split(", ")
            val position = extractThreeVector(split[0])
            val velocity = extractThreeVector(split[1])
            val acceleration = extractThreeVector(split[2])
            particles.add(Particle(index, position, velocity, acceleration))
        }
        return particles
    }

    private fun getInput() = javaClass.getResource("/2017-20.txt").readText(Charsets.UTF_8)

    private fun extractThreeVector(string: String): ThreeVector {
        val center = string.substring(string.indexOf('<') + 1, string.length - 1)
        val values = center.split(',')
        val x = Integer.parseInt(values[0])
        val y = Integer.parseInt(values[1])
        val z = Integer.parseInt(values[2])
        return ThreeVector(x, y, z)
    }

    fun solvePartOne() {
        println(readParticles().minWith(Comparator { a, b ->
            a!!
            b!!
            val aAcceleration = a.acceleration.getNormSquare()
            val bAcceleration = b.acceleration.getNormSquare()
            if (aAcceleration < bAcceleration) {
                return@Comparator -1
            }
            if (aAcceleration > bAcceleration) {
                return@Comparator 1
            }
            val aVelocity = a.velocity.getNormSquare()
            val bVelocity = b.velocity.getNormSquare()
            if (aVelocity < bVelocity) {
                return@Comparator -1
            }
            if (aVelocity > bVelocity) {
                return@Comparator 1
            }
            val aPosition = a.position.getNormSquare()
            val bPosition = b.position.getNormSquare()
            if (aPosition < bPosition) {
                return@Comparator -1
            }
            if (aPosition > bPosition) {
                return@Comparator 1
            }
            0
        })!!.id)
    }

    private fun canCollide(a: Int, b: Int, c: Int): Boolean {
        if (a != 0) {
            // (-b ± sqrt(b^2 - 4ac)) / (2a)
            val delta = b * b - 4 * a * c
            if (delta < 0) return false
            return (-b + Math.sqrt(delta.toDouble())) / (2 * a) >= 0
        } else {
            // -c / b
            if (b == 0) return false
            return -c / b >= 0
        }
    }

    private fun someParticlesCanCollide(particles: List<Particle>): Boolean {
        for (i in 0 until particles.size) {
            for (j in i + 1 until particles.size) {
                val pI = particles[i]
                val pJ = particles[j]
                if (canCollide(pI.acceleration.x - pJ.acceleration.x, pI.velocity.x - pJ.velocity.x, pI.position.x - pJ.position.x)) {
                    if (canCollide(pI.acceleration.y - pJ.acceleration.y, pI.velocity.y - pJ.velocity.y, pI.position.y - pJ.position.y)) {
                        if (canCollide(pI.acceleration.z - pJ.acceleration.z, pI.velocity.z - pJ.velocity.z, pI.position.z - pJ.position.z)) {
                            return true
                        }
                    }
                }
            }
        }
        return false
    }

    fun solvePartTwo() {
        var particles = readParticles()
        val removing = HashSet<Int>()
        while (someParticlesCanCollide(particles)) {
            for (i in 0 until particles.size) {
                particles[i].update()
            }
            for (i in 0 until particles.size) {
                for (j in i + 1 until particles.size) {
                    if (particles[i].position == particles[j].position) {
                        removing.add(particles[i].id)
                        removing.add(particles[j].id)
                    }
                }
            }
            particles = particles.stream().filter { particle -> !removing.contains(particle.id); }.collect(Collectors.toList())
        }
        println(particles.size)
    }
}

fun main(args: Array<String>) {
    val solution = Solution201720()
    solution.solvePartOne()
    solution.solvePartTwo()
}
