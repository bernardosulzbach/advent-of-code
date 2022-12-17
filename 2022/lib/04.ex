defmodule Day04 do
  defp contains?(a, b) do
    a_first..a_last = a
    b_first..b_last = b
    a_first <= b_first and b_last <= a_last
  end

  defp redundant?(a, b) do
    contains?(a, b) or contains?(b, a)
  end

  defp string_to_range(str) do
    [first, last] = String.split(str, "-")
    String.to_integer(first)..String.to_integer(last)
  end

  def run(path, part) do
    with {:ok, contents} = File.read(path) do
      Integer.to_string(
        Enum.count(
          contents
          |> String.split("\n")
          |> Enum.filter(fn s -> s != "" end)
          |> Enum.map(fn line -> Enum.map(String.split(line, ","), &string_to_range/1) end),
          case part do
            1 ->
              fn [a, b] -> redundant?(a, b) end

            2 ->
              fn [a, b] -> not Range.disjoint?(a, b) end
          end
        )
      )
    end
  end
end
