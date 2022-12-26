defmodule Day13 do
  defp compare(a, b) when is_list(a) and is_list(b) do
    case {a, b} do
      {[], []} ->
        :equal

      {[], [_ | _]} ->
        :less

      {[_ | _], []} ->
        :greater

      {[a_hd | a_tl], [b_hd | b_tl]} ->
        case compare(a_hd, b_hd) do
          :equal -> compare(a_tl, b_tl)
          hd_compare -> hd_compare
        end
    end
  end

  defp compare(a, b) when is_list(a) and is_integer(b) do
    compare(a, [b])
  end

  defp compare(a, b) when is_integer(a) and is_list(b) do
    compare([a], b)
  end

  defp compare(a, b) when is_integer(a) and is_integer(b) do
    cond do
      a < b -> :less
      a === b -> :equal
      a > b -> :greater
    end
  end

  def run(path, part) do
    with {:ok, contents} = File.read(path) do
      pairs =
        contents
        |> String.trim()
        |> String.split("\n")
        |> Enum.chunk_by(fn s -> s === "" end)
        |> Enum.reject(fn l -> l === [""] end)
        |> Enum.map(fn pairs ->
          pairs
          |> Enum.map(fn packet ->
            {evaluation, []} = packet |> Code.eval_string()
            evaluation
          end)
        end)

      Integer.to_string(
        case part do
          1 ->
            pairs
            |> Enum.with_index(1)
            |> Enum.map(fn {[a, b], index} ->
              case compare(a, b) do
                :less -> index
                # We don't expect them to compare equal.
                :greater -> 0
              end
            end)
            |> Enum.sum()

          2 ->
            divider_packets = [[[2]], [[6]]]

            (divider_packets ++ (pairs |> Enum.concat()))
            |> Enum.sort(fn a, b -> compare(a, b) !== :greater end)
            |> Enum.with_index(1)
            |> Enum.map(fn {packet, index} ->
              if packet in divider_packets do
                index
              else
                1
              end
            end)
            |> Enum.product()
        end
      )
    end
  end
end
