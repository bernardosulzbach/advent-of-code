defmodule Day25 do
  defp convert_snafu_to_integer(s) do
    s
    |> String.to_charlist()
    |> Enum.reverse()
    |> Enum.with_index()
    |> Enum.map(fn {snafu_digit, index} ->
      case snafu_digit do
        ?2 -> 2
        ?1 -> 1
        ?0 -> 0
        ?- -> -1
        ?= -> -2
      end * 5 ** index
    end)
    |> Enum.sum()
  end

  defp convert_integer_to_snafu(x) do
    if x === 0 do
      "0"
    else
      divide_and_continue = fn x ->
        if x != 0 do
          convert_integer_to_snafu(x)
        else
          ""
        end
      end

      case rem(x, 5) do
        0 -> divide_and_continue.(div(x, 5)) <> "0"
        1 -> divide_and_continue.(div(x - 1, 5)) <> "1"
        2 -> divide_and_continue.(div(x - 2, 5)) <> "2"
        3 -> divide_and_continue.(div(x + 2, 5)) <> "="
        4 -> divide_and_continue.(div(x + 1, 5)) <> "-"
      end
    end
  end

  def run(path, part) do
    with {:ok, contents} = File.read(path) do
      contents
      |> String.trim()
      |> String.split("\n")
      |> Enum.map(&convert_snafu_to_integer/1)
      |> Enum.sum()
      |> convert_integer_to_snafu
    end
  end
end
