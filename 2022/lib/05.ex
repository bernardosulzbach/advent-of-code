defmodule Day05 do
  def run(path, part) do
    with {:ok, contents} = File.read(path) do
      [initial_state, moves] =
        contents
        |> String.split("\n")
        |> Enum.chunk_by(fn s -> s == "" end)
        |> Enum.filter(fn l -> l != [""] end)

      indices =
        initial_state
        |> Enum.reverse()
        |> hd()
        |> String.split()
        |> Enum.map(&String.to_integer/1)

      state =
        initial_state
        |> Enum.reverse()
        |> tl()
        |> Enum.reduce(
          indices |> Map.new(fn x -> {x, []} end),
          fn line, state ->
            line
            |> String.codepoints()
            |> Enum.chunk_every(4)
            |> Enum.with_index(1)
            |> Enum.reduce(state, fn {[first, second | _], index}, state ->
              if first == "[" do
                {_, state} =
                  Map.get_and_update!(state, index, fn list ->
                    {list, [second] ++ list}
                  end)

                state
              else
                state
              end
            end)
          end
        )

      state =
        moves
        |> Enum.reduce(state, fn move, state ->
          [amount, from, to] =
            Enum.drop_every(String.split(move), 2) |> Enum.map(&String.to_integer/1)

          {moved, new_from_stack} = Map.fetch!(state, from) |> Enum.split(amount)

          new_to_stack =
            case part do
              1 -> moved |> Enum.reverse()
              2 -> moved
            end ++ Map.fetch!(state, to)

          %{%{state | from => new_from_stack} | to => new_to_stack}
        end)

      indices |> Enum.map(fn index -> Map.fetch!(state, index) |> hd() end) |> Enum.join()
    end
  end
end
