import Data.Char qualified
import Data.List qualified
import Data.Maybe qualified

longDigits = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]

longStartingDigit s [] = Data.Maybe.Nothing
longStartingDigit s candidates =
  if head candidates `Data.List.isPrefixOf` s
    then Data.Maybe.Just (10 - length candidates)
    else longStartingDigit s (tail candidates)

startingDigit s =
  if Data.Char.isDigit (head s)
    then Data.Maybe.Just (read [head s])
    else longStartingDigit s longDigits

findFirstDigit str = Data.Maybe.fromMaybe (findFirstDigit (tail str)) (startingDigit str)

findLastDigit remaining =
  let findLastDigitLoop remaining current = Data.Maybe.fromMaybe (findLastDigitLoop (tail remaining) (head remaining : current)) (startingDigit current)
   in findLastDigitLoop (tail (reverse remaining)) [last remaining]

calibrationValue str = findFirstDigit str * 10 + findLastDigit str

main = do
  input <- getContents
  print (sum (map calibrationValue (lines input)))
