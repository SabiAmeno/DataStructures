#ifndef _BTREE_INC_H_
#define _BTREE_INC_H_

#include <iostream>
#include "list_inc.h"

#define ds_delete(d) {delete d;d = nullptr;}

/**********************************************
二叉搜索树特点：
    对于任何节点x，x左边的数值都不大于x节点值，x右边的数值不小于x节点值。

三种遍历方式：
    1. 前序遍历
    2. 中序遍历
    3. 后序遍历

**********************************************/

template<typename Type>
struct BstNode
{
    Type data;

    BstNode* P; //父节点
    BstNode* L; //左子节点
    BstNode* R; //右子节点
};

template<typename ValType, template<typename T> typename Node = BstNode>
class BinST
{
public:
    using NS = typename Node<ValType>;
    using Nodes = DNSeqList<Node<ValType>*>;
public:
    BinST() : _root(nullptr){}

    virtual ~BinST()
    {
        if (_root)
        {
            _destroy(_root);
        }
    }

    virtual void insert(const ValType& val)
    {
        _insert(val);
    }

    /*
    删除时可能出现的三种情况：
    1. node没有左孩子，使用右孩子（可能是null）替换node；
    2. node只有一个左孩子，用左孩子替换node；
    3. node若有两个孩子，首先找出右子树的最小值节点x，然后分成两种情况：
        a. 若x为node的右孩子，直接将x替换node即可；
        b. 若x不是node的右孩子，首先使用x.right替换x，再将x移到node的右孩子位置，最后回到情况 a。
    */
    virtual void remove(Node<ValType>* node)
    {
        if (!node->L) {
            _translate(node, node->R);
        } else if(!node->R) {
            _translate(node, node->L);
        } else {
            Node<ValType>* R = _minimum(node->R);
            if (node->R != R) {
                _translate(R, R->R);
                R->R = node->R;
                R->R->P = R;
            }
            _translate(node, R);
            R->L = node->L;
            R->L->P = R;
        }
        ds_delete(node);
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
            preorder_traval(node->L, oss);
            preorder_traval(node->R, oss);
        }
    }

    template<typename Stream>
    void inorder_traval(Node<ValType>* node, Stream& oss)
    {
        if (node)
        {
            inorder_traval(node->L, oss);
            oss << node->data;
            inorder_traval(node->R, oss);
        }
    }

    template<typename Stream>
    void postorder_traval(Node<ValType>* node, Stream& oss)
    {
        if (node)
        {
            postorder_traval(node->L, oss);
            postorder_traval(node->R, oss);
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
                tmp = tmp->L;
            }
            else {
                tmp = tmp->R;
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
        if (node->L)
            return _maximum(node->L);

        Node<ValType>* tmp = node->P;

        while (tmp && node == tmp->L) {
            node = tmp;
            tmp = tmp->P;
        }

        return tmp;
    }

    /*
    和前驱对称
    */
    Node<ValType>* successor(Node<ValType>* node)
    {
        if(node->R) 
            return _minimum(node->R);
        
        Node<ValType>* tmp = node->P;

        while (tmp && node == tmp->R) {
            node = tmp;
            tmp = tmp->P;
        }

        return tmp;
    }

    void children(Nodes& nodes, Node<ValType>* p = nullptr) const {
        nodes.clear();

        if (!p) {
            nodes.push_back(_root);
        } else {
            if (p->L) nodes.push_back(p->L);
            if (p->R) nodes.push_back(p->R);
        }
    }
    
    bool is_empty() const {
        return _root == nullptr;
    }

protected:
    Node<ValType>* _find(const ValType& val)
    {
        Node<ValType>* tmp = _root;
        Node<ValType>* pre = nullptr;

        while (tmp) {
            pre = tmp;
            if (val < tmp->data) {
                tmp = tmp->L;
            } else {
                tmp = tmp->R;
            }
        }

        return pre;
    }

    virtual void _destroy(Node<ValType>* node)
    {
        if (!node)
            return;
        _destroy(node->L);
        _destroy(node->R);

        ds_delete(node);
    }

    Node<ValType>* _insert(const ValType& val)
    {
        Node<ValType>* newNode = new Node<ValType>();
        newNode->data = val;

        if (!_root)
            _root = newNode;
        else {
            //此处查找到的p一定存在
            Node<ValType>* p = _find(val);
            if (p->data > val)
                p->L = newNode;
            else
                p->R = newNode;
            newNode->P = p;
        }

        return newNode;
    }

    Node<ValType>* _minimum(Node<ValType>* node)
    {
        Node<ValType>* tmp = node;

        while (tmp->L)
            tmp = tmp->L;

        return tmp;
    }

    Node<ValType>* _maximum(Node<ValType>* node)
    {
        Node<ValType>* tmp = node;

        while (tmp->R)
            tmp = tmp->R;

        return tmp;
    }

    void _translate(Node<ValType>* u, Node<ValType>* v)
    {
        Node<ValType>* p = u->P;
        if (p)
        {
            if (u == p->L)
                p->L = v;
            else
                p->R = v;
        } else {
            _root = v;
        }
        if (v)
            v->P = u->P;
    }
protected:
    Node<ValType>* _root;
};

#endif
