// xtree internal header

#ifndef XTREE
#define XTREE

enum class Color { Red, Black }; // colors for link to parent

template< typename Ty >
struct TreeNode
{
    using NodePtr = TreeNode*;
    using value_type = Ty;

    NodePtr    left;   // left subtree, or smallest element if head
    NodePtr    parent; // parent, or root of tree if head
    NodePtr    right;  // right subtree, or largest element if head
    Color      color;  // Red or Black, Black if head
    bool       isNil;  // true only if head (also nil) node
    value_type myval;  // the stored value, unused if head
};

// CLASS TEMPLATE TreeVal
template< typename Ty >
class TreeVal
{
public:
    using NodePtr = TreeNode< Ty >*;

    using value_type = Ty;
    using size_type = size_t;

    TreeVal()
        : myHead(new TreeNode< value_type >),
        mySize(0)
    {
        myHead->left = myHead;
        myHead->parent = myHead;
        myHead->right = myHead;
        myHead->color = Color::Black;
        myHead->isNil = true;
    }

    ~TreeVal()
    {
        clear(myHead->parent);
        delete myHead;
    }

    // Removes all elements from the set object (which are destroyed)
    void clear(TreeNode< value_type >* node)
    {
        if (!node->isNil) // node is not an external node
        {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    // rebalance for insertion
    void reBalance(TreeNode< value_type >* node)
    {  // node->parent cannot be the root
      
        TreeNode<value_type>* p = node->parent;
        TreeNode<value_type>* g = p->parent;
        
        if (p == g->right)//R
        {
            if (node == p->right)//RR
            {
                //RR(r) => and parent sibling is red
                if (g->left->color == Color::Red)
                {
                    //parent's sibling red => �u�ݴ���
                    g->color = Color::Red;
                    g->left->color = Color::Black;
                    g->right->color = Color::Black;
                    node = g;
                }
                else//RRb
                {
                    RRRotation(p);//�]������RL��LR������L�@�� �ҥH�n�ǤJparent
                    g->color = Color::Red;
                    p->color = Color::Black;
                    node = p;
                }
            }
            else//RL
            {
                if (g->left->color == Color::Red)//RLr
                {
                    g->color = Color::Red;
                    g->left->color = Color::Black;
                    g->right->color = Color::Black;
                    node = g;//�]��node parent grandparent���w�g�վ㧹�� �ҥH�Y�ϫ���|�X���D�]�O�Hg��node���W��
                }
                else//RLb
                {
                    RLRotation(node);
                    RRRotation(node);//�]��node��W�h�{ �ҥH�n�ǤJparent��RR�@�ˬO�ǤJnode
                    g->color = Color::Red;//grandparent���
                    node->color = Color::Black;//�ۤv���
                }
            }
        }
        else//L
        {
            if (node == p->right)//LR
            {
                if (g->right->color == Color::Red)//LRr
                {
                    g->color = Color::Red;
                    g->left->color = Color::Black;
                    g->right->color = Color::Black;
                    node = g;//�n�O�o��node���W�] �~���~���ˬd�O�_����
                }
                else//LRb
                {
                    LRRotation(node);
                    LLRotation(node);
                    g->color = Color::Red;
                    node->color = Color::Black;
                }
            }
            else//LL
            {
                if (g->right->color == Color::Red)//LLr
                {
                    g->color = Color::Red;
                    g->left->color = Color::Black;
                    g->right->color = Color::Black;
                    node = g;
                }
                else//LLb
                {
                    LLRotation(p); //�]������RL��LR������L�@�� �ҥH�n�ǤJparent
                    g->color = Color::Red;
                    p->color = Color::Black;
                    //node = p;
                }
            }
        }
        ///////////////////////
        if (myHead->parent->color == Color::Red)//�p�Groot�ܦ����� �������
        {
            myHead->parent->color = Color::Black;
        }

        //�p�Gnode�٤��Oroot�åB�O����Bparent�]�O����
        if (node != myHead->parent && node->color == Color::Red && node->parent->color == Color::Red)
        {
            reBalance(node);//�N�A�վ�@��
        }
    }

    // rotate right at g, where p = g->left and node = p->left
    void LLRotation(TreeNode< value_type >* p)
    {
        //g�k��@��
        TreeNode<value_type>* g = p->parent;//g = grandparent node    

        //����W�h���U��(����p)
        if (g->parent == myHead)//�p�Gg�Oroot (root��header��parent����) 
        {
            myHead->parent = p;//p�ܦ�root
            //myHead->left = p;
            //myHead->right = p; //header��left right�n����̤j�̤p
        }
        else if (g == g->parent->right)//g�Droot(���Ynode��right child)
        {
            g->parent->right = p;//p���Ng������right child
        }
        else//g��left child
        {
            g->parent->left = p;//p���Ng������left child
        }

        if (p->right == myHead)//�ݭ쥻p��right���S���l�`�I
        {//�p�G�OmyHead�N��S��
            g->left = myHead;//�ҥH�k��᪺gn��left child����myHead
        }
        else//�쥻p.right���F�誺�� 
        {
            g->left = p->right;//����g��left
            p->right->parent = g;//�쥻p right child��parent�ܦ�gn
        }

        //��p���W��
        p->parent = g->parent;
        //�A��g��p���_��
        g->parent = p;
        p->right = g;
    }

    // LR rotation; p = g->left and node = p->right
    void LRRotation(TreeNode< value_type >* node)
    {
        //����p���� �X�h�Acall LLrotation��(�o�̥����ΰ�)
        TreeNode<value_type>* p = node->parent;//parent
        TreeNode<value_type>* g = p->parent;//grandparent

        node->parent = g;//child����grandparent
        g->left = node;//���_��
        p->parent = node;

        if (node->left == myHead)//if node's left child is nullptr, p's right child point to header
        {
            p->right = myHead;
        }
        else//if isn't empty, move to p's right child
        {
            p->right = node->left;
            node->left->parent = p;//its parent is p
        }
        node->left = p;//connected

    }

    // RL rotation; p = g->right and node = p->left
    void RLRotation(TreeNode< value_type >* node)
    {
        NodePtr p = node->parent;//parent
        NodePtr g = p->parent;//grandparent

        node->parent = g;//child rotate on
        g->right = node;//point to each other
        p->parent = node;

        if (node->right == myHead)
        {
            p->left = myHead;
        }
        else
        {
            p->left = node->right;
            node->right->parent = p;
        }
        node->right = p;
    }

    // rotate left at g, where p = g->right and node = p->right
    void RRRotation(TreeNode< value_type >* p)
    {
        TreeNode<value_type>* g = p->parent;//grandparent
        if (g->parent == myHead)
        {
            myHead->parent = p;
            //
            //
        }
        else if (g == g->parent->right)
        {
            g->parent->right = p;
        }
        else
        {
            g->parent->left = p;
        }

        if (p->left == myHead)
        {
            g->right = myHead;
        }
        else
        {
            g->right = p->left;
            p->left->parent = g;
        }

        p->parent = g->parent;
        g->parent = p;
        p->left = g;
    }

    // erase node provided that the degree of node is at most one
    void eraseDegreeOne(TreeNode< value_type >* node)
    {
        NodePtr child;

        if (node->left==myHead && !node->right->isNil)//temp���k���٦���
        {
            node->right->parent = node->parent;//���ǭȱ��Wtemp��parent
            child = node->right;
        }
        else if (!node->left->isNil && node->right->isNil)
        {
            node->left->parent = node->parent;
            child = node->left;
        }
        else
        {
            child = myHead;//temp�S��l
        }

        if (child->color == Color::Red)//�]�������Dchild����n���W��p�O�ƻ��C��A�ҥH�������¦�C
            //simple case
        {
            child->color = Color::Black;
            node->color = Color::Red;
        }

        if (node->parent!=myHead)//temp�W���Ohead
        {
            if (node->parent->left == node)//�Nchild���Ntemp���Wp
                node->parent->left = child;
            else if (node->parent->right == node)
                node->parent->right = child;
        }
        else//�Ohead
        {
            myHead->parent = child;
            child->parent = myHead;
        }

        if (node->color == Color::Black)
            fixUp(child, node->parent);

        delete node;
        mySize--;
    }

    // rebalance for deletion
    void fixUp(TreeNode< value_type >* N, TreeNode< value_type >* P)//node parent
    {
        NodePtr S;//sibling
        if (P->left == N)//N is left child
        {
            S = P->right;

            if (S->color == Color::Red) // Case 1.1
            {
                S->color = P->color;
                P->color = Color::Red;
                RRRotation(S);
                fixUp(N, P);
            }
            else if (S->right->color == Color::Red) // Case 2.1
            {
                S->color = P->color;
                P->color = Color::Black;
                RRRotation(S);
                S->right->color = Color::Black;
            }
            else if (S->left->color == Color::Red) // Case3.1
            {
                S->color = Color::Red;
                S->left->color = Color::Black;
                this->RLRotation(S->left);
                this->fixUp(N, P);
            }
            else if (P->color == Color::Red) // Case4
            {
                P->color = Color::Black;
                S->color = Color::Red;
            }
            else // Case5
            {
                S->color = Color::Red;
                fixUp(P, P->parent);
            }
        }
        else//N is right child
        {
            S = P->left;

            if (S->color == Color::Red) // Case 1.2
            {
                S->color = P->color;
                P->color = Color::Red;
                LLRotation(S);
                fixUp(N, P);
            }
            else if (S->left->color == Color::Red) // Case 2.2
            {
                S->color = P->color;
                P->color = Color::Black;
                LLRotation(S);
                S->left->color = Color::Black;
            }
            else if (S->right->color == Color::Red) // Case3.2
            {
                S->color = Color::Red;
                S->right->color = Color::Black;
                LRRotation(S->right);
                fixUp(N, P);
            }
            else if (P->color == Color::Red) // Case4
            {
                P->color = Color::Black;
                S->color = Color::Red;
            }
            else // Case5
            {
                S->color = Color::Red;
                fixUp(P, P->parent);
            }
        }
    }



    NodePtr myHead;   // pointer to head node
    size_type mySize; // number of elements
};

// CLASS TEMPLATE Tree
template< typename Traits >
class Tree // ordered red-black tree for map/multimap/set/multiset
{
public:
    using value_type = typename Traits::value_type;

protected:
    using ScaryVal = TreeVal< value_type >;

public:
    using key_type = typename Traits::key_type;
    using key_compare = typename Traits::key_compare;

    using size_type = size_t;

    Tree(const key_compare& parg)
        : keyCompare(parg),
        scaryVal()
    {
    }

    ~Tree()
    {
    }

    // Extends the container by inserting a new element,
    // effectively increasing the container size by one.
    void insert(const value_type& val)
    {
        if (scaryVal.mySize == 0)
        {
            TreeNode< value_type >* root = new TreeNode< value_type >;
            root->myval = val;
            root->left = scaryVal.myHead;
            root->right = scaryVal.myHead;
            root->isNil = false;
            root->parent = scaryVal.myHead;
            root->color = Color::Black;
            scaryVal.myHead->left = root;
            scaryVal.myHead->parent = root;
            scaryVal.myHead->right = root;
            scaryVal.mySize = 1;
        }
        else
        {                           // scaryVal.myHead->parent points to the root
            TreeNode< value_type >* p = scaryVal.myHead->parent;
            TreeNode< value_type >* pp = nullptr;
            while (p != scaryVal.myHead)
            {
                pp = p;
                //              keyCompare.operator()( val, p->myval )
                if (keyCompare(val, p->myval)) // if( val < p->myval )
                    p = p->left;
                else if (keyCompare(p->myval, val)) // if( val > p->myval )
                    p = p->right;
                else
                    return;
            }

            TreeNode< value_type >* node = new TreeNode< value_type >;
            node->myval = val;
            node->left = scaryVal.myHead;
            node->right = scaryVal.myHead;//null
            node->isNil = 0;
            node->parent = pp;//�����W����pp(parent)
            node->color = Color::Red;//insertion�@�w�O����

            if (keyCompare(val, pp->myval))//����p���h��
            {
                pp->left = node;
            }
            else//����j���h�k
            {
                pp->right = node;
            }
            scaryVal.mySize++;//�O�o�n+1
            if (pp->color == Color::Red)
            {
                scaryVal.reBalance(node);
            }
        }
    }

    // Removes from the set container a single element whose value is val
    // This effectively reduces the container size by one, which are destroyed.
    // Returns the number of elements erased.
    size_type erase(const key_type& val)
    {
        TreeNode< key_type >* p = scaryVal.myHead->parent; // p points to the root
        while (p != scaryVal.myHead && val != p->myval)
        {
            //           keyCompare.operator()( val, p->myval )
            if (keyCompare(val, p->myval)) // if( val < p->myval )
                p = p->left;
            else
                p = p->right;
        }

        if (p == scaryVal.myHead) // not found
            return 0;
        else // found
        {  // deletes the node pointed by p
            TreeNode< key_type >* temp = p->right;

            if (p->right != scaryVal.myHead)//right child���Onullptr
            {
                while (temp->left != scaryVal.myHead)//���k�@ �����쩳
                {
                    temp = temp->left;
                }
                p->myval = temp->myval;//copy�W���䪺��node��val��p(�쥻�n�R��node)
                scaryVal.eraseDegreeOne(temp);//�n�R���a��令�Qcopy��node
            }
            else//right child�Onullptr
            {
                scaryVal.eraseDegreeOne(p);//�����R���o�Ӧ�m
            }
            return 1;
        }
    }

private:
    key_compare keyCompare;
    ScaryVal scaryVal;
};

#endif // XTREE




    
