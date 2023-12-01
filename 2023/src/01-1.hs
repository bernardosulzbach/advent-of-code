import Data.Char qualified
import Data.List qualified
import Data.Maybe qualified

calibrationValue s =
  let firstDigit s = Data.Maybe.fromJust (Data.List.find Data.Char.isDigit s)
   in read [firstDigit s, firstDigit (reverse s)]

main = do
  input <- getContents
  print (sum (map calibrationValue (lines input)))
