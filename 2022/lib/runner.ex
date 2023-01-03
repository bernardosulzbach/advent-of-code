defmodule Runner do
  def run() do
    with {:ok, list} <- :application.get_key(:advent_of_code_2022, :modules) do
      list
      |> Enum.filter(fn module ->
        module |> Atom.to_string() |> String.match?(~r/Day\d{2}$/)
      end)
      |> Enum.sort()
      |> Enum.map(fn module ->
        padded_day = module |> Atom.to_string() |> String.slice(-2..-1)
        IO.puts("Day #{padded_day |> String.to_integer()}")

        1..2
        |> Enum.map(fn part ->
          IO.puts("  Part #{part}")

          ["Sample", "Puzzle"]
          |> Enum.map(fn test_type ->
            directory = "#{String.downcase(test_type)}s"

            actual_output =
              module.run(
                "input/#{directory}/#{padded_day}.txt",
                part
              )

            with {:ok, expected_output} =
                   File.read("output/#{directory}/#{padded_day}-#{part}.txt") do
              passed = actual_output === String.trim(expected_output)

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
