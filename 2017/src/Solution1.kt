import java.io.File

fun main(args: Array<String>) {
    val string = File("input/1/input.txt").readText(Charsets.UTF_8).trim()
    val n = string.length
    val sum1 = (0..n - 1)
            .filter { string[it] == string[(it + 1) % n] }
            .sumBy { string[it] - '0' }
    println(sum1)
    val sum2 = (0..n - 1)
            .filter { string[it] == string[(it + n / 2) % n] }
            .sumBy { string[it] - '0' }
    println(sum2)
}