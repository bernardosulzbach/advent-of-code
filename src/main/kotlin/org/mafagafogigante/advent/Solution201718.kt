package org.mafagafogigante.advent

import java.math.BigInteger
import java.util.*

class RegisterOrValue(data: String) {

    val register: Int?
    val value: BigInteger?

    init {
        if (data.isEmpty()) {
            register = null
            value = null
        } else {
            if (data.last().isDigit()) {
                register = null
                value = BigInteger(data)
            } else {
                register = data[0].toInt() - 'a'.toInt()
                value = null
            }
        }
    }

}

class Instruction(val opcode: String, val register: RegisterOrValue, val value: RegisterOrValue)

class Program(id: Int, val instructions: List<Instruction>) {

    val registers = Array<BigInteger>(26, { BigInteger.ZERO })
    val queue = LinkedList<BigInteger>()
    val sent = ArrayList<BigInteger>()
    var ip = 0

    init {
        registers['p'.toInt() - 'a'.toInt()] = BigInteger.valueOf(id.toLong())
    }

    fun isWaiting(): Boolean {
        return instructions[ip].opcode == "rcv" && queue.isEmpty()
    }

    fun finished(): Boolean {
        return ip < 0 || ip >= instructions.size
    }

    fun run() {
        while (!finished()) {
            val instruction = instructions[ip]
            var nextIp = ip + 1
            var value = BigInteger.ZERO
            if (instruction.value.value != null) {
                value = instruction.value.value
            } else if (instruction.value.register != null) {
                value = registers[instruction.value.register]
            }
            if (instruction.opcode == "snd") {
                if (instruction.register.register != null) {
                    sent.add(registers[instruction.register.register])
                } else if (instruction.register.value != null) {
                    sent.add(instruction.register.value)
                }
            } else if (instruction.opcode == "set") {
                registers[instruction.register.register!!] = value
            } else if (instruction.opcode == "add") {
                registers[instruction.register.register!!] += value
            } else if (instruction.opcode == "mul") {
                registers[instruction.register.register!!] *= value
            } else if (instruction.opcode == "mod") {
                registers[instruction.register.register!!] %= value
            } else if (instruction.opcode == "rcv") {
                if (queue.isEmpty()) {
                    return
                }
                registers[instruction.register.register!!] = queue.poll()
            } else if (instruction.opcode == "jgz") {
                var argument = BigInteger.ZERO
                if (instruction.register.register != null) {
                    argument = registers[instruction.register.register]
                } else if (instruction.register.value != null) {
                    argument = instruction.register.value
                }
                if (argument > BigInteger.ZERO) {
                    nextIp = ip + value.toInt()
                }
            }
            ip = nextIp
        }
    }

}

class Solution201718 {

    fun getInput() = javaClass.getResource("/2017-18.txt").readText(Charsets.UTF_8)

    fun getInstructions(text: String) = text.trim().split('\n').map {
        val split = it.trim().split(' ')
        Instruction(split[0], RegisterOrValue(split.getOrElse(1, { "" })), RegisterOrValue(split.getOrElse(2, { "" })))
    }

    fun getFirstSolution(instructions: List<Instruction>): BigInteger {
        var sent = BigInteger.ZERO
        val registers = Array<BigInteger>(26, { BigInteger.ZERO })
        var ip = 0
        while (true) {
            val instruction = instructions[ip]
            var nextIp = ip + 1
            var value = BigInteger.ZERO
            if (instruction.value.value != null) {
                value = instruction.value.value
            } else if (instruction.value.register != null) {
                value = registers[instruction.value.register]
            }
            if (instruction.opcode == "snd") {
                sent = registers[instruction.register.register!!]
            } else if (instruction.opcode == "set") {
                registers[instruction.register.register!!] = value
            } else if (instruction.opcode == "add") {
                registers[instruction.register.register!!] += value
            } else if (instruction.opcode == "mul") {
                registers[instruction.register.register!!] *= value
            } else if (instruction.opcode == "mod") {
                registers[instruction.register.register!!] %= value
            } else if (instruction.opcode == "rcv") {
                if (registers[instruction.register.register!!] != BigInteger.ZERO) {
                    return sent
                }
            } else if (instruction.opcode == "jgz") {
                if (registers[instruction.register.register!!] > BigInteger.ZERO) {
                    nextIp = ip + value.toInt()
                }
            }
            ip = nextIp
        }
    }

    fun getSecondSolution(): Int {
        val instructions = getInstructions(getInput())
        val programA = Program(0, instructions)
        val programB = Program(1, instructions)
        var totalSent = 0
        while (true) {
            programA.run()
            programB.queue.addAll(programA.sent)
            programA.sent.clear()
            if (programA.finished()) {
                break
            }
            programB.run()
            programA.queue.addAll(programB.sent)
            totalSent += programB.sent.size
            programB.sent.clear()
            if (programB.finished()) {
                break
            }
            val deadlock = programA.isWaiting() && programB.isWaiting()
            if (deadlock) {
                break
            }
        }
        return totalSent
    }

}

fun main(args: Array<String>) {
    val solution = Solution201718()
    println(solution.getFirstSolution(solution.getInstructions(solution.getInput())))
    println(solution.getSecondSolution())
}
