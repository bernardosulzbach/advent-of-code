import AoC qualified
import Data.Map qualified
import Data.Maybe qualified
import Day02

colorLimits = Data.Map.fromList [("red", 12), ("green", 13), ("blue", 14)]

isColorWithinLimits (color, count) = count <= Data.Maybe.fromJust (Data.Map.lookup color colorLimits)

isGameWithinLimits (_, colors) = all isColorWithinLimits (Data.Map.toList colors)

main = do
  input <- getContents
  print (sum (map fst (filter isGameWithinLimits (map readGame (lines input)))))