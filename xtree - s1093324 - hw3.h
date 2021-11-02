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
                    //parent's sibling red => 只需換色
                    g->color = Color::Red;
                    g->left->color = Color::Black;
                    g->right->color = Color::Black;
                    node = g;
                }
                else//RRb
                {
                    RRRotation(p);//因為不像RL或LR有先轉過一次 所以要傳入parent
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
                    node = g;//因為node parent grandparent都已經調整完畢 所以即使後續會出問題也是以g為node往上找
                }
                else//RLb
                {
                    RLRotation(node);
                    RRRotation(node);//因為node轉上去ㄌ 所以要傳入parent的RR一樣是傳入node
                    g->color = Color::Red;//grandparent轉紅
                    node->color = Color::Black;//自己轉黑
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
                    node = g;//要記得讓node往上跑 才能繼續檢查是否平衡
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
                    LLRotation(p); //因為不像RL或LR有先轉過一次 所以要傳入parent
                    g->color = Color::Red;
                    p->color = Color::Black;
                    //node = p;
                }
            }
        }
        ///////////////////////
        if (myHead->parent->color == Color::Red)//如果root變成紅的 直接轉黑
        {
            myHead->parent->color = Color::Black;
        }

        //如果node還不是root並且是紅色且parent也是紅色
        if (node != myHead->parent && node->color == Color::Red && node->parent->color == Color::Red)
        {
            reBalance(node);//就再調整一次
        }
    }

    // rotate right at g, where p = g->left and node = p->left
    void LLRotation(TreeNode< value_type >* p)
    {
        //g右轉一次
        TreeNode<value_type>* g = p->parent;//g = grandparent node    

        //先把上層往下接(接到p)
        if (g->parent == myHead)//如果g是root (root跟header的parent互指) 
        {
            myHead->parent = p;//p變成root
            //myHead->left = p;
            //myHead->right = p; //header的left right要指到最大最小
        }
        else if (g == g->parent->right)//g非root(為某node的right child)
        {
            g->parent->right = p;//p取代g成為該right child
        }
        else//g為left child
        {
            g->parent->left = p;//p取代g成為該left child
        }

        if (p->right == myHead)//看原本p的right有沒有子節點
        {//如果是myHead代表沒有
            g->left = myHead;//所以右轉後的gn的left child接到myHead
        }
        else//原本p.right有東西的話 
        {
            g->left = p->right;//接到g的left
            p->right->parent = g;//原本p right child的parent變成gn
        }

        //把p往上接
        p->parent = g->parent;
        //再把g跟p接起來
        g->parent = p;
        p->right = g;
    }

    // LR rotation; p = g->left and node = p->right
    void LRRotation(TreeNode< value_type >* node)
    {
        //先做p左轉 出去再call LLrotation做(這裡先不用做)
        TreeNode<value_type>* p = node->parent;//parent
        TreeNode<value_type>* g = p->parent;//grandparent

        node->parent = g;//child接到grandparent
        g->left = node;//接起來
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

        if (node->left==myHead && !node->right->isNil)//temp的右邊還有值
        {
            node->right->parent = node->parent;//那些值接上temp的parent
            child = node->right;
        }
        else if (!node->left->isNil && node->right->isNil)
        {
            node->left->parent = node->parent;
            child = node->left;
        }
        else
        {
            child = myHead;//temp沒兒子
        }

        if (child->color == Color::Red)//因為不知道child之後要接上的p是甚麼顏色，所以先換成黑色。
            //simple case
        {
            child->color = Color::Black;
            node->color = Color::Red;
        }

        if (node->parent!=myHead)//temp上不是head
        {
            if (node->parent->left == node)//將child取代temp接上p
                node->parent->left = child;
            else if (node->parent->right == node)
                node->parent->right = child;
        }
        else//是head
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
            node->parent = pp;//先往上指到pp(parent)
            node->color = Color::Red;//insertion一定是紅色

            if (keyCompare(val, pp->myval))//比較小的去左
            {
                pp->left = node;
            }
            else//比較大的去右
            {
                pp->right = node;
            }
            scaryVal.mySize++;//記得要+1
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

            if (p->right != scaryVal.myHead)//right child不是nullptr
            {
                while (temp->left != scaryVal.myHead)//往右一 往左到底
                {
                    temp = temp->left;
                }
                p->myval = temp->myval;//copy上面找的的node的val到p(原本要刪的node)
                scaryVal.eraseDegreeOne(temp);//要刪的地方改成被copy的node
            }
            else//right child是nullptr
            {
                scaryVal.eraseDegreeOne(p);//直接刪掉這個位置
            }
            return 1;
        }
    }

private:
    key_compare keyCompare;
    ScaryVal scaryVal;
};

#endif // XTREE




    
