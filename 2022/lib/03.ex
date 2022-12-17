defmodule Day03 do
  defp priority(item_type) do
    cond do
      item_type in ?a..?z -> item_type - ?a + 1
      item_type in ?A..?Z -> item_type - ?A + 27
    end
  end

  def run(path, part) do
    with {:ok, contents} = File.read(path) do
      Integer.to_string(
        Enum.sum(
          case part do
            1 ->
              contents
              |> String.split("\n")
              |> Enum.filter(fn s -> s != "" end)
              |> Enum.map(&String.to_charlist/1)
              |> Enum.map(fn s ->
                half_length = div(length(s), 2)

                [repeated_item_type] =
                  MapSet.to_list(
                    MapSet.intersection(
                      MapSet.new(Enum.slice(s, 0, half_length)),
                      MapSet.new(Enum.slice(s, half_length, half_length))
                    )
                  )

                priority(repeated_item_type)
              end)

            2 ->
              Enum.chunk_every(
                contents
                |> String.split("\n")
                |> Enum.filter(fn s -> s != "" end)
                |> Enum.map(&String.to_charlist/1),
                3
              )
              |> Enum.map(fn rucksacks ->
                [common_item_type] =
                  MapSet.to_list(
                    Enum.reduce(
                      Enum.map(rucksacks, &MapSet.new/1),
                      &MapSet.intersection/2
                    )
                  )

                priority(common_item_type)
              end)
          end
        )
      )
    end
  end
end
