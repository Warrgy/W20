
 Assignment 3. 
 CIS3490
 
 Name = Cameron Fisher
 ID = 1057347
 Date = Mar 9, 2020

 HOW TO RUN:

 1. type: make
 2. type: ./assign3 [algorithm]
    algorithm values:
    1.1 <- brute force number of anagrams
    1.2 <- pre sorting number of anagrams
    2.1 <- brute force string matching
    2.2 <- Horspool Algorithm for string matching.
    2.3 <- Boyer-Moore Algorithm for string matching.
    (ex. "./assign3 1.1" will run the brute force anagram count.)
3. Observe


Analysis for String Matching:
    - After using the strings: {this, university, Guelph, class, school, chicken, sandwich, too, so, academic}
    - The average ratio for pattern shifts to matches are:
    Brute Force:
        - approx 17064 pattern shifts to matches
    Horspool: 
        - approx 6367 pattern shifts to matches
    Boyer-Moore:
        - approx 6030 pattern shifts to matches

    To summarize the results, Boyer-Moore would be most efficient, then Horspool, then brute force. But the Horspool is still very efficient, and is still worthy of being
    used, since you only need one table. But if you have more space available, then Boyer-Moore would be a better choice due to its efficiency. If you are very limited on space
    then Brute-force may lead an alright solution.