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
          ["Sample", "Puzzle"]
          |> Enum.reduce(false, fn test_type, some_output ->
            directory = "#{String.downcase(test_type)}s"

            case File.read("output/#{directory}/#{padded_day}-#{part}.txt") do
              {:ok, expected_output} ->
                actual_output =
                  module.run(
                    "input/#{directory}/#{padded_day}.txt",
                    part
                  )

                passed = actual_output === String.trim(expected_output)

                if !some_output do
                  IO.puts("  Part #{part}")
                end

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

                true

              {:error, :enoent} ->
                false
            end
          end)
        end)
      end)
    end
  end
end
