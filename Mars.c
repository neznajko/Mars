#include <stdio.h>
#include <string.h>
////////////////////////////////////////////////////////
//                                          some globals
// I/O sucks
enum { CLEAR, ROUGH, ROCKSAMPLE };
#define P 4 // width
#define Q 4 // height
typedef int terrain_t[Q][P];
terrain_t grid = {
  {0, 2, 0, 2},
  {2, 1, 0, 2},
  {0, 0, 2, 2},
  {1, 2, 0, 0},
};
terrain_t dist;
void dist_init( void )
{
  memset( dist, 0xff, sizeof dist ); // -1
  **dist = 0;
}
////////////////////////////////////////////////////////
void dump_terrain( terrain_t t, int w )
{
  static char buf[BUFSIZ];
  static char fmt[] = "%_d";
  fmt[1] = '0' + w;

  for ( int i = 0; i < Q; ++i ) {
    *buf = 0;
    for ( int j = 0; j < P; ++j ) {
      snprintf( buf + w*j, BUFSIZ, fmt, t[i][j] );
    }
    printf( "[%s]\n", buf );
  }
}
////////////////////////////////////////////////////////
typedef struct vtx {
  int coor[2];
} vtx_t;

char *vtx2str( vtx_t *v )
{
  static char buf[] = "(--,--)";

  snprintf( buf, sizeof buf, "(%2d,%2d)", v->coor[0], v->coor[1] );

  return buf;
}

vtx_t prev[Q][P];

void prev_init( void )
{
  memset( prev, 0xff, sizeof prev );
}

void dump_prev( void )
{
  static char buf[BUFSIZ];

  for ( int i = 0; i < Q; ++i ) {
    *buf = 0;
    for ( int j = 0; j < P; ++j ) {
      strcat( buf, vtx2str( &prev[i][j] ));
    }
    puts( buf );
  }
}
////////////////////////////////////////////////////////////////
// * Heap Section * //
int bp = 0;        // heap's back pointer
vtx_t heap[1+P*Q]; // base-1 max heap (key: dist)
void svap( int i, int j )
{
  static vtx_t v;
  v = heap[i]; heap[i] = heap[j]; heap[j] = v;
}
int get( terrain_t t, vtx_t *v )
{
  return t[v->coor[0]][v->coor[1]];
}
int lt( int i, int j )
{
  return get( dist, &heap[i] ) < get( dist, &heap[j] );
}
void bubble( int j )
{
  int p; // parent

  while ( p = j >> 1 ) {
    if ( lt( p, j )) {
      svap( p, j );
      j = p;
    } else break;
  }
}
void insert( int i, int j )
{
  heap[++bp] = (vtx_t) { i, j };
  bubble( bp );
}
void makeheap()
{
  bp = 0;
  for ( int i = 0; i < Q; ++i ) {
    for ( int j = 0; j < P; ++j ) {
      if ( grid[i][j] != ROUGH ) {
        insert( i, j );
      }
    }
  }
}
int maxchild( int p )
{
  int l = p << 1; // left child

  if ( l == bp ) {
    return l;
  } else if ( l > bp ) {
    return -1;
  } else {
    int r = l + 1;
    if ( lt( l, r )) {
      return r;
    } else {
      return l;
    }
  }
}
vtx_t *pop()
{
  svap( 1, bp-- );

  int p = 1, c;

  for (;;) {
    if (( c = maxchild( p )) == -1 || lt( c, p )) {
      break;
    }
    svap( p, c );
    p = c;
  }
  return &heap[bp+1];
}
int find( vtx_t *v )
{
  for ( int j = 1; j <= bp; ++j ) {
    vtx_t *u = &heap[j];
    for ( int i = 0; i <= 1; ++i ) {
      if ( u->coor[i] != v->coor[i] ) {
        goto skip;
      }
    }
    return j;
skip:;
  }
  return -1;
}
void update( vtx_t *v )
{
  int j = find( v );
  bubble( j );
}
void dumpheap()
{
  if ( bp == 0 ) puts( "empty heap" );
  int level = 1; // 1, 3, 7, 15, ...
  static char buf[BUFSIZ] = "";
  static char info[] = "(--,--)[**]";
  for ( int j = 1; j <= bp; ++j ) {
    vtx_t *v = &heap[j];
    snprintf( info, sizeof info, "%s[%d]", vtx2str( v ), get( dist, v ));
    strcat( buf, info );
    if ( j == level || j == bp ) {
      puts( buf );
      *buf = 0;
      level = ( level << 1 ) + 1;
    }
  }
}
// Proper Dijkstra's
typedef enum { SOUTH, EAST } dir_t;
int backtrace()
{
  static int scv = 1;
  int samples = 0;
  int i = Q-1;
  int j = P-1;
  for (;;) {
    vtx_t *p = &prev[i][j];
    i = p->coor[0];
    j = p->coor[1];
    if ( i == -1 ) break;
    if ( get( grid, p ) == ROCKSAMPLE ) {
      ++samples;
      grid[i][j] = CLEAR;
    }
  }
  if ( samples ) {
    printf( "scv %d: %d samples\n", scv++, samples );
  }
  return samples;
}
int dijkstra()
{
  static int shape[] = { Q, P };

  dist_init();
  prev_init();
  makeheap();
  dump_terrain( grid, 2 );
  while (bp > 0) {
    vtx_t *v = pop();
    for ( dir_t d = SOUTH; d <= EAST; ++d ) {
      vtx_t u = *v;
      ++u.coor[d];
      if (u.coor[d] == shape[d] || get( grid, &u ) == 1 ) {
        continue;
      }
      int shortcut = get( dist, v ) + get( grid, v ); 
      if ( get( dist, &u ) < shortcut ) {
        int i = u.coor[0];
        int j = u.coor[1];
        dist[i][j] = shortcut;
        prev[i][j] = *v;
        update( &u );
      }
    }
  }
  return backtrace();
}
int main( void )
{
  while (dijkstra())
    ;
}
// log:
