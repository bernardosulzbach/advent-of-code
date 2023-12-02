module AoC (splitOn, splitOnFirst) where

splitOn separator list =
  let f a b
        | null b = [a]
        | head b == separator = a : splitOn separator (tail b)
        | otherwise = f (a ++ [head b]) (tail b)
   in f [] list

splitOnFirst separator list =
  let f a b = if head b == separator then (a, tail b) else f (a ++ [head b]) (tail b)
   in f [] list