defmodule Runner do
  def run() do
    [Day01, Day02, Day03, Day04]
    |> Enum.with_index()
    |> Enum.map(fn {module, index} -> {module, index + 1} end)
    |> Enum.map(fn {module, day} ->
      IO.puts("Day #{day}")

      1..2
      |> Enum.map(fn part ->
        IO.puts("  Part #{part}")

        ["Sample", "Puzzle"]
        |> Enum.map(fn test_type ->
          padded_day = day |> Integer.to_string() |> String.pad_leading(2, "0")
          directory = "#{String.downcase(test_type)}s"

          actual_output =
            module.run(
              "input/#{directory}/#{padded_day}.txt",
              part
            )

          with {:ok, expected_output} = File.read("output/#{directory}/#{padded_day}-#{part}.txt") do
            passed = actual_output == String.slice(expected_output, 0..-2)

            IO.puts(
              if passed do
                IO.ANSI.green()
              else
                IO.ANSI.red()
              end <>
                "    #{test_type} " <>
                if passed do
                  "passed."
                else
                  "failed!"
                end <>
                IO.ANSI.reset()
            )
          end
        end)
      end)
    end)
  end
end
