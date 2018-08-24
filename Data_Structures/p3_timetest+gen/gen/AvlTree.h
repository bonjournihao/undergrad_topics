        #ifndef _AVL_TREE_H_
        #define _AVL_TREE_H_

        #include "dsexceptions.h"
#include <stdlib.h> // for NULL
          // Node and forward declaration because g++ does
          // not understand nested classes.
        template <class Comparable>
        class AvlTree;

        template <class Comparable>
        class AvlNode
        {
            Comparable element;
            AvlNode   *left;
            AvlNode   *right;
            int        height;

            AvlNode( Comparable & theElement, AvlNode *lt, AvlNode *rt, int h = 0 )
              : element( theElement ), left( lt ), right( rt ), height( h ) { }
            friend class AvlTree<Comparable>;
        };


        // AvlTree class
        //
        // CONSTRUCTION: with ITEM_NOT_FOUND object used to signal failed finds
        //
        // ******************PUBLIC OPERATIONS*********************
        // void insert( x )       --> Insert x
        // void remove( x )       --> Remove x (unimplemented)
        // Comparable find( x )   --> Return item that matches x
        // Comparable findMin( )  --> Return smallest item
        // Comparable findMax( )  --> Return largest item
        // boolean isEmpty( )     --> Return true if empty; else false
        // void makeEmpty( )      --> Remove all items
        // void printTree( )      --> Print tree in sorted order

        template <class Comparable>
        class AvlTree
        {
          public:
            explicit AvlTree( Comparable & notFound );
            AvlTree( const AvlTree & rhs );
            ~AvlTree( );

            const Comparable & findMin( ) const;
            const Comparable & findMax( ) const;
            Comparable & find( Comparable & x );
            bool isEmpty( ) const;
            void printTree( ) const;

            void makeEmpty( );
            void insert( Comparable & x );
            void remove( Comparable & x );

            const AvlTree & operator=( const AvlTree & rhs );

          private:
            AvlNode<Comparable> *root;
            Comparable ITEM_NOT_FOUND;

            Comparable & elementAt( AvlNode<Comparable> *t );

            void insert( Comparable & x, AvlNode<Comparable> * & t ) const;
            void remove( Comparable & x, AvlNode<Comparable> * & t) const;
            AvlNode<Comparable> * findMin( AvlNode<Comparable> *t ) const;
            AvlNode<Comparable> * findMax( AvlNode<Comparable> *t ) const;
            AvlNode<Comparable> * find( Comparable & x, AvlNode<Comparable> *t ) const;
            void makeEmpty( AvlNode<Comparable> * & t ) const;
            void printTree( AvlNode<Comparable> *t ) const;
            AvlNode<Comparable> * clone( AvlNode<Comparable> *t ) const;

                // Avl manipulations
            int height( AvlNode<Comparable> *t ) const;
            int max( int lhs, int rhs ) const;
            void rotateWithLeftChild( AvlNode<Comparable> * & k2 ) const;
            void rotateWithRightChild( AvlNode<Comparable> * & k1 ) const;
            void doubleWithLeftChild( AvlNode<Comparable> * & k3 ) const;
            void doubleWithRightChild( AvlNode<Comparable> * & k1 ) const;
        };

        #include "AvlTree.cpp"
        #endif
