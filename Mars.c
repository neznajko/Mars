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
  {2, 0, 1, 2},
  {1, 0, 2, 0},
  {0, 2, 0, 0},
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
  int i;
  int j;
} vtx_t;

char *vtx2str( vtx_t v )
{
  static char buf[] = "(--,--)";

  snprintf( buf, sizeof buf, "(%02d,%02d)", v.i, v.j );

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
      strcat( buf, vtx2str( prev[i][j] ));
    }
    puts( buf );
  }
}
////////////////////////////////////////////////////////////////
// * Heap Section * //
int hp = P*Q;     // heap pointer (back to front and back)
vtx_t heap[1+hp]; // base-1 max heap (key: dist)
void svap( int i, int j )
{
}
//
int main( void )
{
  prev_init();
  dump_prev();
}
// log: 
