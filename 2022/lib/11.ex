defmodule Day11 do
  defmodule Monkey do
    use TypedStruct

    typedstruct do
      field(:items, list(non_neg_integer), enforce: true)
      field(:operation, (non_neg_integer -> non_neg_integer), enforce: true)
      field(:divisor, pos_integer(), enforce: true)
      field(:next, %{false: non_neg_integer(), true: non_neg_integer()}, enforce: true)
      field(:inspections, non_neg_integer(), default: 0)
    end
  end

  @spec parse_monkey(list(String.t())) :: Monkey.t()
  defp parse_monkey(lines) do
    [
      "Monkey " <> _,
      "  Starting items: " <> items,
      "  Operation: new = old " <> operation,
      "  Test: divisible by " <> divisor,
      "    If true: throw to monkey " <> next_if_true,
      "    If false: throw to monkey " <> next_if_false
    ] = lines

    %Monkey{
      items:
        items
        |> String.split(", ")
        |> Enum.map(&String.to_integer/1),
      operation: fn old ->
        [operator, rhs] = operation |> String.split(" ")

        (case operator do
           "+" -> &+/2
           "*" -> &*/2
         end).(
          old,
          case rhs do
            "old" -> old
            s -> String.to_integer(s)
          end
        )
      end,
      divisor: String.to_integer(divisor),
      next: %{
        true: String.to_integer(next_if_true),
        false: String.to_integer(next_if_false)
      }
    }
  end

  def run(path, part) do
    with {:ok, contents} = File.read(path) do
      monkeys =
        contents
        |> String.trim()
        |> String.split("\n\n")
        |> Enum.map(fn monkey_string ->
          monkey_string |> String.split("\n") |> parse_monkey()
        end)

      # TODO: here, too, I'd like a "projection" transform
      lcm = monkeys |> Enum.map(fn monkey -> monkey.divisor end) |> Enum.reduce(&Math.lcm/2)

      1..case part do
        1 -> 20
        2 -> 10000
      end
      |> Enum.reduce(
        monkeys,
        # TODO: can I stop getting round here?
        fn _round, monkeys ->
          0..((monkeys |> Enum.count()) - 1)
          |> Enum.reduce(
            monkeys,
            fn index, monkeys ->
              monkey = monkeys |> Enum.at(index)

              monkeys =
                monkey.items
                |> Enum.reduce(monkeys, fn old_item_level, monkeys ->
                  new_item_level =
                    case part do
                      1 ->
                        div(monkey.operation.(old_item_level), 3)

                      2 ->
                        rem(monkey.operation.(old_item_level), lcm)
                    end

                  # TODO: maybe use arrays so we can have get_and_update et al.?
                  next_monkey_index = monkey.next[rem(new_item_level, monkey.divisor) === 0]
                  next_monkey = monkeys |> Enum.at(next_monkey_index)

                  List.replace_at(monkeys, next_monkey_index, %Monkey{
                    next_monkey
                    | items: next_monkey.items ++ [new_item_level]
                  })
                end)

              List.replace_at(monkeys, index, %Monkey{
                monkey
                | items: [],
                  inspections: monkey.inspections + (monkey.items |> Enum.count())
              })
            end
          )
        end
      )
      # TODO: is there a transform that takes a member?
      |> Enum.map(fn monkey -> monkey.inspections end)
      |> Enum.sort(:desc)
      |> Enum.take(2)
      |> Enum.product()
      |> Integer.to_string()
    end
  end
end
