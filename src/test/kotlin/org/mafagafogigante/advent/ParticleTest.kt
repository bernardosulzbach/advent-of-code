package org.mafagafogigante.advent

import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

internal class ParticleTest {

    @Test
    fun testStepsBeforeAccelerationOnlyAccelerates() {
        val position = Point3D.ORIGIN
        val acceleration = Point3D(2, 0, 0)
        val particleA = Particle(position, Point3D(-3, 0, 0), acceleration, 0)
        Assertions.assertEquals(2, particleA.stepsBeforeAccelerationOnlyAccelerates())
        val particleB = Particle(position, Point3D(-2, 0, 0), acceleration, 0)
        Assertions.assertEquals(1, particleB.stepsBeforeAccelerationOnlyAccelerates())
        val particleC = Particle(position, Point3D(-1, 0, 0), acceleration, 0)
        Assertions.assertEquals(1, particleC.stepsBeforeAccelerationOnlyAccelerates())
        val particleD = Particle(position, Point3D(0, 0, 0), acceleration, 0)
        Assertions.assertEquals(0, particleD.stepsBeforeAccelerationOnlyAccelerates())
        val particleE = Particle(position, Point3D(1, 0, 0), acceleration, 0)
        Assertions.assertEquals(0, particleE.stepsBeforeAccelerationOnlyAccelerates())
    }

}