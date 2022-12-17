defmodule Day02 do
  defp translate(l) do
    cond do
      l == "A" or l == "X" -> :rock
      l == "B" or l == "Y" -> :paper
      l == "C" or l == "Z" -> :scissors
    end
  end

  defp pick(s, l) do
    cond do
      l == "X" -> inferior(s)
      l == "Y" -> s
      l == "Z" -> superior(s)
    end
  end

  defp superior(a) do
    case a do
      :scissors -> :rock
      :rock -> :paper
      :paper -> :scissors
    end
  end

  defp inferior(a) do
    case a do
      :rock -> :scissors
      :paper -> :rock
      :scissors -> :paper
    end
  end

  defp score(a) do
    case a do
      :rock -> 1
      :paper -> 2
      :scissors -> 3
    end
  end

  defp score(a, b) do
    cond do
      a == inferior(b) -> 6
      a == b -> 3
      a == superior(b) -> 0
    end + score(b)
  end

  def run(path, part) do
    with {:ok, contents} = File.read(path) do
      Integer.to_string(
        case part do
          1 ->
            with scores =
                   contents
                   |> String.split("\n")
                   |> Enum.filter(fn s -> s != "" end)
                   |> Enum.map(fn s ->
                     score(translate(String.at(s, 0)), translate(String.at(s, 2)))
                   end) do
              Enum.sum(scores)
            end

          2 ->
            with scores =
                   contents
                   |> String.split("\n")
                   |> Enum.filter(fn s -> s != "" end)
                   |> Enum.map(fn s ->
                     adversary_move = translate(String.at(s, 0))
                     score(adversary_move, pick(adversary_move, String.at(s, 2)))
                   end) do
              Enum.sum(scores)
            end
        end
      )
    end
  end
end
