import AoC
import Data.Map qualified
import Data.Maybe qualified
import Day02

main = do
  input <- getContents
  print (sum (map (((product . map snd . Data.Map.toList) . snd) . readGame) (lines input)))
