defmodule Day10 do
  def run(path, part) do
    with {:ok, contents} = File.read(path) do
      width = 40
      height = 6

      values =
        contents
        |> String.trim()
        |> String.split("\n")
        |> Enum.reduce([1], fn instruction, values ->
          x = values |> hd

          case instruction do
            "noop" -> [x | values]
            "addx " <> number -> [x + String.to_integer(number), x | values]
          end
        end)
        |> Enum.reverse()
        |> Enum.take(width * height)

      case part do
        1 ->
          values
          |> Enum.with_index(1)
          |> Enum.filter(fn {_, index} -> rem(index - 20, width) === 0 end)
          |> Enum.map(fn {x, index} -> x * index end)
          |> Enum.sum()
          |> Integer.to_string()

        2 ->
          values
          |> Enum.chunk_every(width)
          |> Enum.map(fn
            chunk ->
              chunk
              |> Enum.with_index(0)
              |> Enum.map(fn {x, index} ->
                if abs(x - index) <= 1 do
                  "#"
                else
                  "."
                end
              end)
              |> Enum.join()
          end)
          |> Enum.join("\n")
      end
    end
  end
end
