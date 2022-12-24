defmodule Day06 do
  def run(path, part) do
    with {:ok, contents} = File.read(path) do
      contents
      |> String.trim()
      |> String.split("\n")
      |> Enum.map(fn line ->
        required_distinct =
          case part do
            1 -> 4
            2 -> 14
          end

        line
        |> String.codepoints()
        |> Enum.chunk_every(required_distinct, 1, :discard)
        |> Enum.with_index()
        # Can be done more efficiently by XORing a bit for each character into an accumulator and stopping when the number of set bits reaches to the value we want.
        |> Enum.filter(fn {values, _} ->
          values == values |> Enum.uniq()
        end)
        |> Enum.map(fn {_, index} ->
          index + required_distinct
        end)
        |> Enum.at(0)
      end)
      |> Enum.map(&Integer.to_string/1)
      |> Enum.join("\n")
    end
  end
end
