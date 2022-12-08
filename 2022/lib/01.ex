defmodule Day01 do
  def run(path, part) do
    with {:ok, contents} = File.read(path) do
      with calories =
             contents
             |> String.split("\n")
             |> Enum.chunk_by(fn s -> s == "" end)
             |> Enum.filter(fn l -> l != [""] end)
             |> Enum.map(fn l -> Enum.sum(l |> Enum.map(&String.to_integer/1)) end) do
        Integer.to_string(
          case part do
            1 ->
              Enum.max(calories)

            2 ->
              Enum.sum(
                Enum.take(
                  Enum.sort(calories),
                  -3
                )
              )
          end
        )
      end
    end
  end
end
