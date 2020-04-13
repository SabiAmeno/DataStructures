#ifndef _BTREE_INC_H_
#define _BTREE_INC_H_

#include <iostream>

/**********************************************
�����������ص㣺
    �����κνڵ�x��x��ߵ���ֵ��������x�ڵ�ֵ��x�ұߵ���ֵ��С��x�ڵ�ֵ��

���ֱ�����ʽ��
    1. ǰ�����
    2. �������
    3. �������

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
            //�˴����ҵ���pһ������
            Node<ValType>* p = _find(val);
            if (p->data > val)
                p->left = newNode;
            else
                p->right = newNode;
            newNode->parent = p;
        }
    }

    /*
    ɾ��ʱ���ܳ��ֵ����������
    1. nodeû�����ӣ�ʹ���Һ��ӣ�������null���滻node��
    2. nodeֻ��һ�����ӣ��������滻node��
    3. node�����������ӣ������ҳ�����������Сֵ�ڵ�x��Ȼ��ֳ����������
        a. ��xΪnode���Һ��ӣ�ֱ�ӽ�x�滻node���ɣ�
        b. ��x����node���Һ��ӣ�����ʹ��x.right�滻x���ٽ�x�Ƶ�node���Һ���λ�ã����ص���� a��
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

    /*ǰ��
    1. ���node�����ӽڵ㣬ǰ�����������������ֵ�ڵ�
    2. ��nodeû�����ӽڵ��£������丸�ڵ㣬ֱ��ĳ�� ���Ƚڵ� �� �����Ƚڵ� �� ���ڵ� �� �Һ��� Ϊֹ��
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
    ��ǰ���Գ�
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
