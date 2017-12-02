import java.io.File

fun main(args: Array<String>) {
    var checksum = 0
    for (line in File("input/2/input.txt").readLines(Charsets.UTF_8)) {
        var min = Integer.MAX_VALUE
        var max = Integer.MIN_VALUE
        for (bit in line.split("\t")) {
            min = Math.min(min, bit.toInt())
            max = Math.max(max, bit.toInt())
        }
        checksum += max - min
    }
    println(checksum)
    var sum = 0
    for (line in File("input/2/input.txt").readLines(Charsets.UTF_8)) {
        val integers = ArrayList<Int>()
        line.split("\t").mapTo(integers) { it.toInt() }
        for (i in 0..integers.size - 1) {
            for (j in i + 1..integers.size - 1) {
                if (integers[i] % integers[j] == 0) {
                    sum += integers[i] / integers[j]
                }
                if (integers[j] % integers[i] == 0) {
                    sum += integers[j] / integers[i]
                }
            }
        }
    }
    println(sum)
}