defmodule Day08 do
  def run(path, part) do
    with {:ok, contents} = File.read(path) do
      heights_matrix =
        contents
        |> String.trim()
        |> String.split("\n")
        |> Enum.map(fn line ->
          line |> String.codepoints() |> Enum.map(&String.to_integer/1)
        end)

      zip_apply_and_apply_transformed = fn enum, transform, fun ->
        Enum.zip(
          enum |> fun.(),
          enum |> transform.() |> fun.() |> transform.()
        )
      end

      nearest_matrix =
        zip_apply_and_apply_transformed.(
          heights_matrix,
          &AdventOfCode.Common.transpose/1,
          fn matrix ->
            matrix
            |> Enum.map(fn heights ->
              heights
              |> Enum.with_index()
              |> zip_apply_and_apply_transformed.(&Enum.reverse/1, fn indexed_heights ->
                [
                  %{}
                  | indexed_heights
                    |> Enum.drop(-1)
                    |> Enum.scan(%{}, fn {height, index}, last_seen_of_height ->
                      last_seen_of_height |> Map.put(height, index)
                    end)
                ]
              end)
            end)
          end
        )
        |> Enum.map(fn {x_nearest, y_nearest} ->
          Enum.zip(x_nearest, y_nearest)
        end)

      Integer.to_string(
        case part do
          1 ->
            Enum.zip(heights_matrix, nearest_matrix)
            |> Enum.map(fn {heights, nearest} ->
              Enum.zip(heights, nearest)
              |> Enum.map(fn {height,
                              {{x_nearest_before, x_nearest_after},
                               {y_nearest_before, y_nearest_after}}} ->
                all_visible = fn nearest ->
                  nearest
                  |> Map.keys()
                  |> Enum.filter(&(&1 >= height))
                  |> Enum.empty?()
                end

                all_visible.(x_nearest_before) ||
                  all_visible.(x_nearest_after) ||
                  all_visible.(y_nearest_before) ||
                  all_visible.(y_nearest_after)
              end)
            end)
            |> Enum.map(fn row -> row |> Enum.count(& &1) end)
            |> Enum.sum()

          2 ->
            patch_width = heights_matrix |> hd |> Enum.count()
            patch_height = heights_matrix |> Enum.count()

            Enum.zip(heights_matrix, nearest_matrix)
            |> Enum.with_index()
            |> Enum.map(fn {{heights, nearest}, y_index} ->
              Enum.zip(heights, nearest)
              |> Enum.with_index()
              |> Enum.map(fn {{height,
                               {{x_nearest_before, x_nearest_after},
                                {y_nearest_before, y_nearest_after}}}, x_index} ->
                count_visible = fn nearest, index, empty_fallback ->
                  nearest
                  |> Map.keys()
                  |> Enum.filter(&(&1 >= height))
                  |> Enum.map(fn key -> abs(index - Map.fetch!(nearest, key)) end)
                  |> Enum.min(empty_fallback)
                end

                count_visible.(x_nearest_before, x_index, fn -> x_index end) *
                  count_visible.(x_nearest_after, x_index, fn ->
                    patch_width - 1 - x_index
                  end) *
                  count_visible.(y_nearest_before, y_index, fn -> y_index end) *
                  count_visible.(y_nearest_after, y_index, fn ->
                    patch_height - 1 - y_index
                  end)
              end)
            end)
            |> Enum.map(&Enum.max/1)
            |> Enum.max()
        end
      )
    end
  end
end
