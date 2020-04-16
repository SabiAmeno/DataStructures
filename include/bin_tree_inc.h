#ifndef _BTREE_INC_H_
#define _BTREE_INC_H_

#include <iostream>
#include "list_inc.h"

#define ds_delete(d) {delete d;d = nullptr;}

/**********************************************
�����������ص㣺
    �����κνڵ�x��x��ߵ���ֵ��������x�ڵ�ֵ��x�ұߵ���ֵ��С��x�ڵ�ֵ��

���ֱ�����ʽ��
    1. ǰ�����
    2. �������
    3. �������

**********************************************/

template<typename Type>
struct BstNode
{
    Type data;

    BstNode* P; //���ڵ�
    BstNode* L; //���ӽڵ�
    BstNode* R; //���ӽڵ�
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
    ɾ��ʱ���ܳ��ֵ����������
    1. nodeû�����ӣ�ʹ���Һ��ӣ�������null���滻node��
    2. nodeֻ��һ�����ӣ��������滻node��
    3. node�����������ӣ������ҳ�����������Сֵ�ڵ�x��Ȼ��ֳ����������
        a. ��xΪnode���Һ��ӣ�ֱ�ӽ�x�滻node���ɣ�
        b. ��x����node���Һ��ӣ�����ʹ��x.right�滻x���ٽ�x�Ƶ�node���Һ���λ�ã����ص���� a��
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

    /*ǰ��
    1. ���node�����ӽڵ㣬ǰ�����������������ֵ�ڵ�
    2. ��nodeû�����ӽڵ��£������丸�ڵ㣬ֱ��ĳ�� ���Ƚڵ� �� �����Ƚڵ� �� ���ڵ� �� �Һ��� Ϊֹ��
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
    ��ǰ���Գ�
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
            //�˴����ҵ���pһ������
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
