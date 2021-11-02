#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;

#include <set>
#include "set.h"

template< typename T, typename T1 = less< T >, typename T2 = std::less< T > >
void testSet();

// returns true if and only if set1 == set2
template< typename T, typename T1, typename T2 >
bool equal( set< T, T1 > &set1, std::set< T, T2 > &set2 );

// returns true if and only if 
// the subtree rooted at node1 is equal to the subtree rooted at node2
template< typename Kty >
bool equal( size_t *node1, size_t *head1,
            size_t *node2, size_t *head2 );

unsigned long long int twoTo32 = static_cast< unsigned long long int >( ULONG_MAX ) + 1;

int main()
{
   for( unsigned int seed = 1; seed <= 10; seed++ )
   {
      srand( seed );
      testSet< unsigned int >();
   }

   for( unsigned int seed = 1; seed <= 10; seed++ )
   {
      srand( seed );
      testSet< unsigned int, greater< unsigned int >, std::greater< unsigned int > >();
   }

   for( unsigned int seed = 11; seed <= 20; seed++ )
   {
      srand( seed );
      testSet< unsigned short >();
   }

   for( unsigned int seed = 11; seed <= 20; seed++ )
   {
      srand( seed );
      testSet< unsigned short, greater< unsigned short >, std::greater< unsigned short > >();
   }

   system( "pause" );
}

template< typename T, typename T1, typename T2 >
void testSet()
{
   unsigned int numKeys = 1000;
   unsigned int key;

      set< T, T1 > set1;
      std::set< T, T2 > set2;

      unsigned int numErrors = numKeys + 1;
      if( equal( set1, set2 ) )
         numErrors--;

      for( unsigned int i = 0; i < numKeys; i++ )
      {
         switch( rand() % 3 )
         {
         case 0:
         case 1:
            key = 1 + rand() % ( 5 * numKeys );
            set1.insert( key );
            set2.insert( key );
            break;
         case 2:
            if( set2.size() > 0 )
            {
               key = 1 + rand() % ( 5 * numKeys );
               set1.erase( key );
               set2.erase( key );
            }
         }

         if( equal( set1, set2 ) )
            numErrors--;
      }

      cout << "There are " << numErrors << " errors.\n";
}

// returns true if and only if set1 == set2
template< typename T, typename T1, typename T2 >
bool equal( set< T, T1 > &set1, std::set< T, T2 > &set2 )
{
   size_t size1 = *( reinterpret_cast< size_t * >( &set1 ) + 2 );
   size_t size2 = *( reinterpret_cast< size_t * >( &set2 ) + 2 );

   if( size1 != size2 )
      return false;

   size_t *head1 = *( reinterpret_cast< size_t ** >( &set1 ) + 1 );
   size_t *head2 = *( reinterpret_cast< size_t ** >( &set2 ) + 1 );

   size_t color1 = *( head1 + 3 ) % twoTo32;
   size_t color2 = *( head2 + 3 ) % 256;

   if( color1 != color2 )
      return false;

   bool isNil1;
   if( sizeof( size_t ) == 8 ) // x64 platform
      isNil1 = static_cast< bool >( *( head1 + 3 ) / twoTo32 % 256 );
   else // x86 platform
      isNil1 = static_cast< bool >( *( head1 + 4 ) % 256 );
   bool isNil2 = static_cast< bool >( *( head2 + 3 ) / 256 % 256 );

   if( isNil1 != isNil2 )
      return false;

   if( size1 == 0 )
      return true;

   size_t *root1 = *( reinterpret_cast< size_t ** >( head1 ) + 1 );
   size_t *root2 = *( reinterpret_cast< size_t ** >( head2 ) + 1 );
   return equal< T >( root1, head1, root2, head2 );
}

// returns true if and only if 
// the subtree rooted at node1 is equal to the subtree rooted at node2
template< typename Kty >
bool equal( size_t *node1, size_t *head1,
            size_t *node2, size_t *head2 )
{
   // node1 == head1 if and only if node1 is an external node
   // node2 == head2 if and only if node2 is an external node
   if( node1 == head1 && node2 == head2 )
      return true;

   if( node1 == head1 && node2 != head2 )
      return false;

   if( node1 != head1 && node2 == head2 )
      return false;

   Kty myVal1;
   Kty myVal2;
   if( sizeof( size_t ) == 8 ) // x64 platform
   {
      myVal1 = static_cast< Kty >( *( node1 + 4 ) % twoTo32 );
      myVal2 = static_cast< Kty >( *( node2 + 3 ) / twoTo32 );
   }
   else // x86 platform
   {
      myVal1 = static_cast< Kty >( *( node1 + 5 ) );
      myVal2 = static_cast< Kty >( *( node2 + 4 ) );
   }
   if( myVal1 != myVal2 )
      return false;

   size_t color1 = *( node1 + 3 ) % twoTo32;
   size_t color2 = *( node2 + 3 ) % 256;
   if( color1 != color2 )
      return false;

   bool isNil1;
   if( sizeof( size_t ) == 8 ) // x64 platform
      isNil1 = static_cast< bool >( *( node1 + 3 ) / twoTo32 % 256 );
   else // x86 platform
      isNil1 = static_cast< bool >( *( node1 + 4 ) % 256 );
   bool isNil2 = static_cast< bool >( *( node2 + 3 ) / 256 % 256 );
   if( isNil1 != isNil2 )
      return false;

   size_t *leftChild1 = *( reinterpret_cast< size_t ** >( node1 ) );
   size_t *rightChild1 = *( reinterpret_cast< size_t ** >( node1 ) + 2 );
   size_t *leftChild2 = *( reinterpret_cast< size_t ** >( node2 ) );
   size_t *rightChild2 = *( reinterpret_cast< size_t ** >( node2 ) + 2 );

   return equal< Kty >( leftChild1, head1, leftChild2, head2 ) &&
          equal< Kty >( rightChild1, head1, rightChild2, head2 );
}