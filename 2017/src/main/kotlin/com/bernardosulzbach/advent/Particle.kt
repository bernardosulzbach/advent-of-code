package com.bernardosulzbach.advent

class Particle(val id: Int, val position: ThreeVector, val velocity: ThreeVector, val acceleration: ThreeVector) {

    var invalid = false

    override fun toString(): String {
        return "Particle(id=$id, position=$position, velocity=$velocity, acceleration=$acceleration)"
    }

    fun update() {
        velocity += acceleration
        position += velocity
        if (position.invalid) {
            invalid = true
        }
    }

}
