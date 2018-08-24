    #include "AvlTree.h"
    #include <iostream>
    using namespace std;

    /**
     * Implements an unbalanced Avl search tree.
     * Note that all "matching" is based on the compares method.
     * @author Mark Allen Weiss
     */
        /**
         * Construct the tree.
         */
        
        AvlTree::AvlTree( vertex * notFound ) :
          root(NULL), ITEM_NOT_FOUND( notFound )
        {
        }

        /**
         * Copy constructor.
         */
        
        AvlTree::AvlTree( AvlTree & rhs ) :
          root(NULL), ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND )
        {
           *this = rhs;
        }

        /**
         * Destructor for the tree.
         */
        
        AvlTree::~AvlTree( )
        {
            makeEmpty( );
        }

        /**
         * Insert x into the tree; duplicates are ignored.
         */
        
        void AvlTree::insert( vertex *x )
        {
            insert( x, root );
        }

/**
* Remove x from the tree. Nothing is done if x is not found.
*/


void AvlTree::remove( vertex *x )
{
 	remove(x, root);
}


/**
* Remove x from the tree. Nothing is done if x is not found.
* Written by Sean Davis 10/17/00 
*/

void AvlTree::remove(vertex *x, AvlNode * & t) 
{
  if( t == NULL ) // never found x
    return; // do nothing
  else
    if( x->dist < t->element->dist )
      remove( x, t->left);
    else
      if( t->element->dist < x->dist )
        remove( x, t->right);
      else // Found x
      {
        if(t->right != NULL && t->left != NULL)
        {
          t->element = findMin(t->right)->element;
            // set this node to minimum of right subtree
          remove(t->element, t->right);  // remove the element we just duplicated
        }  // if two children
        else // one or no children
        {
          AvlNode * temp = NULL; // default is being a leaf
          if(t->left)  // only left child
            temp = t->left;
          else
            if(t->right) // only right child
              temp = t->right;

          delete t;
          t = temp;
        }  // else less than two children
      } // found x
  // Now check and restore AVL property.  This is done for whole path from root.
  if(t)
  {
    if( height( t->left ) - height( t->right ) == 2 )  // left subtree too high
    {
      if( height(t->left->left) >  height(t->left->right))
        rotateWithLeftChild( t );
      else
        doubleWithLeftChild( t );
    }
    else
      if( height( t->left ) - height( t->right ) == -2 ) // right subtree too high
      {
        if( height(t->right->right) > height(t->right->left))
          rotateWithRightChild( t );
        else
          doubleWithRightChild( t );
      }
  }
  if(t)
    t->height = max(height(t->left), height( t->right)) + 1;
} // remove()

        /**
         * Find the smallest item in the tree.
         * Return smallest item or ITEM_NOT_FOUND if empty.
         */
        
        vertex* AvlTree::findMin( )
        {
            return elementAt( findMin( root ) );
        }

        /**
         * Find the largest item in the tree.
         * Return the largest item of ITEM_NOT_FOUND if empty.
         */
        
        vertex* AvlTree::findMax( )
        {
            return elementAt( findMax( root ) );
        }

        /**
         * Find item x in the tree.
         * Return the matching item or ITEM_NOT_FOUND if not found.
         */
        
        vertex* AvlTree::
                                 find( vertex * x )
        {
            return elementAt( find( x, root ) );
        }

        /**
         * Make the tree logically empty.
         */
        
        void AvlTree::makeEmpty( )
        {
            makeEmpty( root );
        }

        /**
         * Test if the tree is logically empty.
         * Return true if empty, false otherwise.
         */
        
        bool AvlTree::isEmpty( ) 
        {
            return root == NULL;
        }

        /**
         * Print the tree contents in sorted order.
         */
        
        void AvlTree::printTree( ) 
        {
            if( isEmpty( ) )
                cout << "Empty tree" << endl;
            else
                printTree( root );
        }

        /**
         * Deep copy.
         */
        
        const AvlTree &
        AvlTree::
        operator=( const AvlTree & rhs )
        {
            if( this != &rhs )
            {
                makeEmpty( );
                root = clone( rhs.root );
            }
            return *this;
        }

        /**
         * Internal method to get element field in node t.
         * Return the element field or ITEM_NOT_FOUND if t is NULL.
         */
        
        vertex* AvlTree::elementAt( AvlNode *t ) 
        {
            return t == NULL ? ITEM_NOT_FOUND : t->element;
        }

        /**
         * Internal method to insert into a subtree.
         * x is the item to insert.
         * t is the node that roots the tree.
         */
        
        void AvlTree::insert( vertex * x, AvlNode * & t )
        {
            if( t == NULL )
                t = new AvlNode( x, NULL, NULL );

            cout << "avl" << endl;
            
            else if( x->dist < t->element->dist )
            {
                insert( x, t->left );
                if( height( t->left ) - height( t->right ) == 2 )
                {
                    if( x->dist < t->left->element->dist )
                        rotateWithLeftChild( t );
                    else
                        doubleWithLeftChild( t );
                }
            }
            else if( t->element->dist < x->dist )
            {
                insert( x, t->right );
                if( height( t->right ) - height( t->left ) == 2 )
                {
                    if( t->right->element->dist < x->dist )
                        rotateWithRightChild( t );
                    else
                        doubleWithRightChild( t );
                }
            }
            else
                ;  // Duplicate; do nothing
            t->height = max( height( t->left ), height( t->right ) ) + 1;
        }

        /**
         * Internal method to find the smallest item in a subtree t.
         * Return node containing the smallest item.
         */
        
        AvlNode *
        AvlTree::findMin( AvlNode *t ) 
        {
            if( t == NULL)
                return t;

            while( t->left != NULL )
                t = t->left;
            return t;
        }

        /**
         * Internal method to find the largest item in a subtree t.
         * Return node containing the largest item.
         */
        
        AvlNode *
        AvlTree::findMax( AvlNode *t ) 
        {
            if( t == NULL )
                return t;

            while( t->right != NULL )
                t = t->right;
            return t;
        }

        /**
         * Internal method to find an item in a subtree.
         * x is item to search for.
         * t is the node that roots the tree.
         * Return node containing the matched item.
         */
        
        AvlNode *
        AvlTree::find( vertex * x, AvlNode *t ) 
        {
            while( t != NULL )
                if( x->dist < t->element->dist )
                    t = t->left;
                else if( t->element->dist < x->dist )
                    t = t->right;
                else
                    return t;    // Match

            return NULL;   // No match
        }

        /**
         * Internal method to make subtree empty.
         */
        
        void AvlTree::makeEmpty( AvlNode * & t ) 
        {
            if( t != NULL )
            {
                makeEmpty( t->left );
                makeEmpty( t->right );
                delete t;
            }
            t = NULL;
        }

        /**
         * Internal method to clone subtree.
         */
        
        AvlNode *
        AvlTree::clone( AvlNode * t ) 
        {
            if( t == NULL )
                return NULL;
            else
                return new AvlNode( t->element, clone( t->left ),
                                              clone( t->right ), t->height );
        }

        /**
         * Return the height of node t, or -1, if NULL.
         */
        
        int AvlTree::height( AvlNode *t ) 
        {
            return t == NULL ? -1 : t->height;
        }

        /**
         * Return maximum of lhs and rhs.
         */
        
        int AvlTree::max( int lhs, int rhs ) 
        {
            return lhs > rhs ? lhs : rhs;
        }

        /**
         * Rotate binary tree node with left child.
         * For AVL trees, this is a single rotation for case 1.
         * Update heights, then set new root.
         */
        
        void AvlTree::rotateWithLeftChild( AvlNode * & k2 ) 
        {
            AvlNode *k1 = k2->left;
            k2->left = k1->right;
            k1->right = k2;
            k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
            k1->height = max( height( k1->left ), k2->height ) + 1;
            k2 = k1;
        }

        /**
         * Rotate binary tree node with right child.
         * For AVL trees, this is a single rotation for case 4.
         * Update heights, then set new root.
         */
        
        void AvlTree::rotateWithRightChild( AvlNode * & k1 ) 
        {
            AvlNode *k2 = k1->right;
            k1->right = k2->left;
            k2->left = k1;
            k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
            k2->height = max( height( k2->right ), k1->height ) + 1;
            k1 = k2;
        }

        /**
         * Double rotate binary tree node: first left child.
         * with its right child; then node k3 with new left child.
         * For AVL trees, this is a double rotation for case 2.
         * Update heights, then set new root.
         */
        
        void AvlTree::doubleWithLeftChild( AvlNode * & k3 ) 
        {
            rotateWithRightChild( k3->left );
            rotateWithLeftChild( k3 );
        }

        /**
         * Double rotate binary tree node: first right child.
         * with its left child; then node k1 with new right child.
         * For AVL trees, this is a double rotation for case 3.
         * Update heights, then set new root.
         */
        
        void AvlTree::doubleWithRightChild( AvlNode * & k1 ) 
        {
            rotateWithLeftChild( k1->right );
            rotateWithRightChild( k1 );
        }

        /**
         * Internal method to print a subtree in sorted order.
         * t points to the node that roots the tree.
         */
        
        void AvlTree::printTree( AvlNode *t ) 
        {
            if( t != NULL )
            {
                printTree( t->left );
                cout << t->element->dist << endl;
                printTree( t->right );
            }
        }
