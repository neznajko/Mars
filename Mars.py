#!/usr/bin/env python3
import numpy as np
class Heap:
    """
    Max heap using distance as key. Here we can think of terrain
    as edges weight, so we can find the path with maximum length.
    """
    def __init__( self, dist ):
        self.key = dist         # reference (numpy array)
        self.arr = [( -1, -1 )] # 1-based heap
        for vtx, value in np.ndenumerate( dist ):
            # skip rough terrain
            if value != 1: self.insert( vtx )

    def __str__( self ):
        if self.empty(): return "The Heap is empty!"
        levl = 1 # 1,3,7,15,...
        buff = ""
        back = len( self.arr ) - 1 # Last Mohican
        j = 0
        while j < back:
            j += 1
            v = self.arr[j]
            k = self.key[v]
            buff += "({},{})[{}]".format( *v, k )
            if j == levl or j == back:
                buff += '\n'
                levl = ( levl << 1 ) + 1
        return buff

    def swap( self, i, j ):
        self.arr[i], self.arr[j] = self.arr[j], self.arr[i]

    def lt( self, i, j ):
        return (self.key[self.arr[i]] < self.key[self.arr[j]])

    def bubbleup( self, j ):
        while True:
            p = j // 2 # parent node
            if p == 0: break
            if self.lt( p, j ): # Does parent vtx have less dist?
                self.swap( p, j )
                j = p
            else: break

    def insert( self, vtx ):
        self.arr.append( vtx )
        self.bubbleup( len( self.arr ) - 1 )

    def find( self, vtx ):
        for j in range( 1, len( self.arr )):
            if vtx == self.arr[j]: return j
        return -1

    def update( self, vtx ):
        """
        Here vtx dist has changed, and we have to update the heap.
        """
        j = self.find( vtx )
        self.bubbleup( j )

    def maxchild( self, j ):
        l = j << 1              # left child (2j)
        n = len( self.arr ) - 1 # last index
        if l > n:
            return -1
        if l == n:
            return l
        else:
            r = l + 1           # right child
            if self.lt( l, r ): # left < right?
                return r
            else:
                return l
        
    def pop( self ):
        """ Ck if empty before popping! """
        front = self.arr[1]
        self.swap( 1, -1 ) # back to front
        self.arr.pop() 
        # sink down the backman
        j = 1
        while True:
            c = self.maxchild( j )
            if c == -1: break
            if self.lt( j, c ): # parent < child?
                self.swap( j, c )
                j = c
            else: break
        return front

    def empty( self ):
        ''' Ck if empty. '''
        return len( self.arr ) == 1

def dijkstra( grid ):
    '''
    '''
    # mark rough terrains to avoid queueing them
    dist = np.full( grid.shape, -1, dtype=int )
    dist[grid == 1] = 1
    dist[0, 0] = 0
    # this is the output, starting from the last vertex
    # it will give backtrace of scv's path
    prev = np.full( grid.shape, None )
    # create a priority queue using distances as keys.
    heap = Heap( dist )
    # loop until all points are visited
    while not heap.empty():
        # pop up the front
        u = heap.pop()
        # ck East\South nbors
        for j in 0, 1:
            # copy to list so we can modify it
            v = list( u )
            # moo in the given direction
            v[j] += 1
            # ck if out of bounds
            if v[j] == dist.shape[j]: continue
            # back to normal
            v = tuple( v )
            # avoid rough terrain
            if grid[v] == 1: continue
            # update distance if necessary
            if dist[v] < dist[u] + grid[u]:
                dist[v] = dist[u] + grid[u]
                prev[v] = u
                heap.update( v )            
    return prev        

def backtrace( scv, prev, grid ):
    v = prev[-1, -1]
    s = ""
    samples = 0
    print(grid)
    while v:
        if grid[v] == 2:
            samples += 1
            s += '[{},{}], '.format( *v )
            grid[v] = 0
        else:
            s += '({},{}), '.format( *v )
        v = prev[v]
    if 0 < samples:
        print( "SCV: {}, Samples: {}".format( scv, samples ))
        print( "Backtrace:", s[:-2] )
    return samples

def main():
    """
    """
    # load grid array from input file
    grid = np.loadtxt( 'NPUT', dtype=int )
    # initialize scv unit counter
    scv = 0
    # run dijkstra algorithm until no moar rock samples
    while True:
        prev = dijkstra( grid )
        # ck if done
        scv += 1
        samples = backtrace( scv, prev, grid )
        if samples == 0: break

if __name__ == '__main__':
    main()

# log:
