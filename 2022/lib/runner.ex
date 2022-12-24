defmodule Runner do
  def run() do
    with {:ok, list} <- :application.get_key(:advent_of_code_2022, :modules) do
      list
      |> Enum.filter(fn module ->
        module |> Atom.to_string() |> String.match?(~r/Day\d{2}/)
      end)
      |> Enum.sort()
      |> Enum.map(fn module ->
        day = module |> Atom.to_string() |> String.slice(-2..-1) |> String.to_integer()
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

            with {:ok, expected_output} =
                   File.read("output/#{directory}/#{padded_day}-#{part}.txt") do
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
end
