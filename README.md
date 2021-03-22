## Mars

*T h i s* is my first Dijkstra's Algorithm.
We have a map of Mars Terrain represented as a 2D array, with each
element being either CLEAR, ROUGH or ROCK SAMPLE terrain. The 
upper left corner is the ***staat*** and the lower right is the 
**finish**. We are given a bunch of robots, gathering rock samples,
and are asked to program those units to gather as much samples as
possible: [clck](https://ioinformatics.org/files/ioi1997problem1.pdf).

It's easy to count the combinatorics, if we have an *nxn* map, the 
length of path is *2(n-1)*, half of the directions being East, 
the other half South, and the number of all possible paths will be
**C<sub>2(n-1)</sub><sup>n-1</sup>=[2(n-1)]!/[(n-1)!]<sup>2</sup>**.
For *n=255* this number is:
```Python
2965051733181991309
3939428137883505420
1838487625300532630
3448679689732858601
0552773308212988238
1908285232672147145
6049947336656893240
0654255201713409920
```
We can think of terrain as vertices of a graph, and the
Rock Samples as having links going to or from them with bigger
weights. Thus the problem is reduced to that, how to find the 
maximum length path, vhich is Dijkstra's Algorithm. So the first
SCV will gather as much possible Rock Samples as SCV can gather
from one go. Than using the *prev* link we can backtrace and mark
explored Rock Samples as Clear Terrain, and than we let go the second
SCV and so on until we are out of SCVs or Rock Samples. Here is the
output from the Python program:
```Python
[[0 2 0 0 0 2 0 0]  <- This is the map 
 [0 2 1 2 0 0 0 0]     0 - clear
 [0 0 2 1 2 2 0 1]     1 - rough
 [0 1 0 1 0 0 0 2]     2 - rock sample
 [2 2 0 0 0 0 2 0]
 [0 1 2 0 2 0 1 0]                       This is the Backtrace
 [0 2 0 1 0 1 0 0]                           |
 [0 1 0 0 0 0 2 0]]                          V
SCV: 1, Samples: 6                       
Backtrace: [7,6], (7,5), (7,4), (6,4), [5,4], (5,3), [5,2], (4,2), (3,2), [2,2], (2,1), [1,1], [0,1], (0,0)
[[0 0 0 0 0 2 0 0]
 [0 0 1 2 0 0 0 0]
 [0 0 0 1 2 2 0 1]
 [0 1 0 1 0 0 0 2]
 [2 2 0 0 0 0 2 0]  <- Here all 2's from unit 1 path are set to 0
 [0 1 0 0 0 0 1 0]
 [0 2 0 1 0 1 0 0]
 [0 1 0 0 0 0 0 0]]
SCV: 2, Samples: 4
Backtrace: (6,7), (5,7), (4,7), [4,6], (4,5), (3,5), [2,5], [2,4], (1,4), [1,3], (0,3), (0,2), (0,1), (0,0)
[[0 0 0 0 0 2 0 0]
 [0 0 1 0 0 0 0 0]
 [0 0 0 1 0 0 0 1]
 [0 1 0 1 0 0 0 2]      If we are given only 3 robots than we stop here
 [2 2 0 0 0 0 0 0]      with 12 gathered samples.
 [0 1 0 0 0 0 1 0]
 [0 2 0 1 0 1 0 0]
 [0 1 0 0 0 0 0 0]]
SCV: 3, Samples: 2
Backtrace: (7,6), (7,5), (7,4), (7,3), (7,2), (6,2), (5,2), (4,2), [4,1], [4,0], (3,0), (2,0), (1,0), (0,0)
[[0 0 0 0 0 2 0 0]
 [0 0 1 0 0 0 0 0]
 [0 0 0 1 0 0 0 1]
 [0 1 0 1 0 0 0 2]
 [0 0 0 0 0 0 0 0]
 [0 1 0 0 0 0 1 0]
 [0 2 0 1 0 1 0 0]
 [0 1 0 0 0 0 0 0]]
SCV: 4, Samples: 2
Backtrace: (6,7), (5,7), (4,7), [3,7], (3,6), (2,6), (2,5), (1,5), [0,5], (0,4), (0,3), (0,2), (0,1), (0,0)
[[0 0 0 0 0 0 0 0]
 [0 0 1 0 0 0 0 0]
 [0 0 0 1 0 0 0 1]
 [0 1 0 1 0 0 0 0]
 [0 0 0 0 0 0 0 0]
 [0 1 0 0 0 0 1 0]
 [0 2 0 1 0 1 0 0]
 [0 1 0 0 0 0 0 0]]
SCV: 5, Samples: 1
Backtrace: (7,6), (7,5), (7,4), (7,3), (7,2), (6,2), [6,1], (6,0), (5,0), (4,0), (3,0), (2,0), (1,0), (0,0)
[[0 0 0 0 0 0 0 0]
 [0 0 1 0 0 0 0 0]
 [0 0 0 1 0 0 0 1]
 [0 1 0 1 0 0 0 0]       If there are 6+ SCVs than we stop here, cos all 
 [0 0 0 0 0 0 0 0]       terrain has been explored.
 [0 1 0 0 0 0 1 0]
 [0 0 0 1 0 1 0 0]
 [0 1 0 0 0 0 0 0]]
```
