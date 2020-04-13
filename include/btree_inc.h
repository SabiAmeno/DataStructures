#ifndef _BTREE_INC_H_
#define _BTREE_INC_H_

#include <iostream>

/**********************************************
二叉搜索树特点：
    对于任何节点x，x左边的数值都不大于x节点值，x右边的数值不小于x节点值。

三种遍历方式：
    1. 前序遍历
    2. 中序遍历
    3. 后序遍历

**********************************************/

template<typename Type>
struct Node
{
    Type data;

    Node* parent;
    Node* left;
    Node* right;
};

template<typename ValType>
class BinST
{
public:
    BinST() : _root(nullptr){}

    ~BinST()
    {
        if (_root)
        {
            _destroy(_root);
        }
    }

    void insert(const ValType& val)
    {
        Node<ValType>* newNode = new Node<ValType>();
        newNode->data = val;

        if (!_root)
            _root = newNode;
        else {
            //此处查找到的p一定存在
            Node<ValType>* p = _find(val);
            if (p->data > val)
                p->left = newNode;
            else
                p->right = newNode;
            newNode->parent = p;
        }
    }

    /*
    删除时可能出现的三种情况：
    1. node没有左孩子，使用右孩子（可能是null）替换node；
    2. node只有一个左孩子，用左孩子替换node；
    3. node若有两个孩子，首先找出右子树的最小值节点x，然后分成两种情况：
        a. 若x为node的右孩子，直接将x替换node即可；
        b. 若x不是node的右孩子，首先使用x.right替换x，再将x移到node的右孩子位置，最后回到情况 a。
    */
    void remove(Node<ValType>* node)
    {
        if (!node->left) {
            _translate(node, node->right);
        } else if(!node->right) {
            _translate(node, node->left);
        } else {
            Node<ValType>* right = _minimum(node->right);
            if (node->right != right) {
                _translate(right, right->right);
                right->right = node->right;
                right->right->parent = right;
            }
            _translate(node, right);
            right->left = node->left;
            right->left->parent = right;
        }
    }

    void remove(ValType val)
    {
        Node<ValType>* node = search(val);
        if (node)
            remove(node);
    }

    template<typename Stream>
    void preorder_traval(Node<ValType>* node, Stream& oss)
    {
        if (node)
        {
            oss << node->data;
            preorder_traval(node->left, oss);
            preorder_traval(node->right, oss);
        }
    }

    template<typename Stream>
    void inorder_traval(Node<ValType>* node, Stream& oss)
    {
        if (node)
        {
            inorder_traval(node->left, oss);
            oss << node->data;
            inorder_traval(node->right, oss);
        }
    }

    template<typename Stream>
    void postorder_traval(Node<ValType>* node, Stream& oss)
    {
        if (node)
        {
            postorder_traval(node->left, oss);
            postorder_traval(node->right, oss);
            oss << node->data;
        }
    }

    Node<ValType>* search(const ValType& val)
    {
        Node<ValType>* tmp = _root;

        while (tmp) {
            if (val == tmp->data)
                break;
            if (val < tmp->data) {
                tmp = tmp->left;
            }
            else {
                tmp = tmp->right;
            }
        }

        return tmp;
    }

    Node<ValType>* minimum()
    {
        return _minimum(_root);
    }

    Node<ValType>* maximum()
    {
        return _maximum(_root);
    }

    /*前驱
    1. 如果node有左子节点，前驱就是左子树的最大值节点
    2. 在node没有左子节点下，回溯其父节点，直到某个 祖先节点 是 该祖先节点 的 父节点 的 右孩子 为止。
    */
    Node<ValType>* predecessor(Node<ValType>* node)
    {
        if (node->left)
            return _maximum(node->left);

        Node<ValType>* tmp = node->parent;

        while (tmp && node == tmp->left) {
            node = tmp;
            tmp = tmp->parent;
        }

        return tmp;
    }

    /*
    和前驱对称
    */
    Node<ValType>* successor(Node<ValType>* node)
    {
        if(node->right) 
            return _minimum(node->right);
        
        Node<ValType>* tmp = node->parent;

        while (tmp && node == tmp->right) {
            node = tmp;
            tmp = tmp->parent;
        }

        return tmp;
    }
private:
    Node<ValType>* _find(const ValType& val)
    {
        Node<ValType>* tmp = _root;
        Node<ValType>* pre = nullptr;

        while (tmp) {
            pre = tmp;
            if (val < tmp->data) {
                tmp = tmp->left;
            } else {
                tmp = tmp->right;
            }
        }

        return pre;
    }

    void _destroy(Node<ValType>* node)
    {
        if (!node)
            return;
        _destroy(node->left);
        _destroy(node->right);

        delete node;
    }

    Node<ValType>* _minimum(Node<ValType>* node)
    {
        Node<ValType>* tmp = node;

        while (tmp->left)
            tmp = tmp->left;

        return tmp;
    }

    Node<ValType>* _maximum(Node<ValType>* node)
    {
        Node<ValType>* tmp = node;

        while (tmp->right)
            tmp = tmp->right;

        return tmp;
    }

    void _translate(Node<ValType>* u, Node<ValType>* v)
    {
        Node<ValType>* p = u->parent;
        if (p)
        {
            if (u == p->left)
                p->left = v;
            else
                p->right = v;
        } else {
            _root = v;
        }
        if (v)
            v->parent = nullptr;
    }
private:
    Node<ValType>* _root;
};

#endif
