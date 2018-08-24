        #ifndef _AVL_TREE_H_
        #define _AVL_TREE_H_

        #include "dsexceptions.h"
#include <stdlib.h> // for NULL
          // Node and forward declaration because g++ does
          // not understand nested classes.
        class AvlTree;

        class AvlNode
        {
            vertex *element;
            AvlNode   *left;
            AvlNode   *right;
            int        height;

            AvlNode( vertex * theElement, AvlNode *lt, AvlNode *rt, int h = 0 )
              : element( theElement ), left( lt ), right( rt ), height( h ) { }
            friend class AvlTree;
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

        class AvlTree
        {
          public:
            explicit AvlTree( vertex* notFound );
            AvlTree(AvlTree & rhs );
            ~AvlTree( );

            vertex* findMin( );
            vertex* findMax( );
            vertex* find( vertex * x );
            bool isEmpty( ) ;
            void printTree( ) ;

            void makeEmpty( );
            void insert( vertex * x );
            void remove( vertex * x );

            const AvlTree & operator=( const AvlTree & rhs );

          private:
            AvlNode* root;
            vertex *ITEM_NOT_FOUND;

            vertex* elementAt( AvlNode *t );

            void insert( vertex * x, AvlNode * & t ) ;
            void remove( vertex * x, AvlNode * & t) ;
            AvlNode* findMin( AvlNode *t ) ;
            AvlNode* findMax( AvlNode *t ) ;
            AvlNode* find( vertex * x, AvlNode *t ) ;
            void makeEmpty( AvlNode * & t ) ;
            void printTree( AvlNode *t ) ;
            AvlNode* clone( AvlNode *t ) ;

                // Avl manipulations
            int height( AvlNode*t ) ;
            int max( int lhs, int rhs ) ;
            void rotateWithLeftChild( AvlNode * & k2 ) ;
            void rotateWithRightChild( AvlNode* & k1 ) ;
            void doubleWithLeftChild( AvlNode* & k3 ) ;
            void doubleWithRightChild( AvlNode* & k1 ) ;
        };

        #include "AvlTree.cpp"
        #endif
