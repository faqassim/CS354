LRU cache simulator which accounts for hits misses and evictions 

//reference simulator is an executable provided by instructor for code testing purposes.

SAMPLE OUTPUT: 
                      Your simulator     Reference simulator
Points (s,E,b)    Hits  Misses  Evicts    Hits  Misses  Evicts
     3 (1,1,1)       9       8       6       9       8       6  traces/trace1
     3 (4,2,4)       4       5       2       4       5       2  traces/trace2
     3 (2,1,4)       2       3       1       2       3       1  traces/trace3
     3 (2,1,3)     167      71      67     167      71      67  traces/trace4
     3 (2,2,3)     201      37      29     201      37      29  traces/trace4
     3 (2,4,3)     212      26      10     212      26      10  traces/trace4
     3 (5,1,5)     231       7       0     231       7       0  traces/trace4
     6 (5,1,5)  265189   21775   21743  265189   21775   21743  traces/trace5
