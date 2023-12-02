module Day02 (readCounts, readGame) where

import AoC
import Data.Map qualified

readCounts str =
  let readCount s = let [count, color] = words s in (color, read count :: Int)
   in Data.Map.fromList (map readCount (AoC.splitOn ',' str))

readGame str =
  let (a, b) = AoC.splitOnFirst ':' str
   in (read (last (words a)) :: Int, Data.Map.unionsWith max (map readCounts (AoC.splitOn ';' b)))