defmodule AdventOfCode.Common do
  def transpose(enum) do
    enum |> Enum.zip() |> Enum.map(&Tuple.to_list/1)
  end
end
