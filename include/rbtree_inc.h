#ifndef _RBTREE_INC_H_
#define _RBTREE_INC_H_

#include "btree_inc.h"


/*****************************************
��������ʣ�
    1. ÿ���ڵ��Ǻ�ɫ���ɫ
    2. ���ڵ��Ǻ�ɫ��
    3. ÿ��Ҷ�ڵ��Ǻ�ɫ��
    4. ���һ���ڵ��Ǻ�ɫ�ģ������������ӽڵ㶼�Ǻ�ɫ��
    5. ÿ���ڵ㵽�����ӽڵ��·���ϣ���������ͬ��Ŀ�ĺ�ɫ�ڵ�


******************************************/
enum class NodeColor
{
    NC_RED,
    NC_BLACK
};

template<typename Type>
struct RBNode
{
    Type      data;
    NodeColor color;

    RBNode* P;
    RBNode* L;
    RBNode* R;

    RBNode(NodeColor c = NodeColor::NC_RED) 
        : color(c), P(0), L(0), R(0)
    {}
};

#define isL(v) ((v) == (v->P->L))
#define isR(v) ((v) == (v->P->R))

#define setL(u, v) (u)->L = (v)
#define setR(u, v) (u)->R = (v)
#define setP(u, v) (u)->P = (v)

#define setPL(u, v) (u)->P->L = (v) 
#define setPR(u, v) (u)->P->R = (v)
#define setRP(u, v) (u)->R->P = (v)
#define setLP(u, v) (u)->L->P = (v)

#define isRED(u) (NodeColor::NC_RED == (u)->color)
#define isBLK(u) (NodeColor::NC_BLACK == (u)->color)

#define setCLR(u, c) (u)->color = (c)
#define setRED(u) setCLR(u, NodeColor::NC_RED)
#define setBLK(u) setCLR(u, NodeColor::NC_BLACK)

template<typename ValType, template<typename T> typename Node = RBNode>
class RedBTree : public BinST<ValType, Node>
{
public:
    virtual void insert(const ValType& val)
    {
        Node<ValType>* nnode = _insert(val);

        _insert_fixup(nnode);
    }

    virtual void remove(ValType val)
    {
        Node<ValType>* node = search(val);
        if (node)
            remove(node);
    }

    virtual void remove(Node<ValType>* node)
    {
        Node<ValType>* y = node;
        //��¼ɾ���Ľڵ���������ڲ��ƶ��Ľڵ����ɫ�����Ǻ�ɫ������Ҫ���е���
        NodeColor old_y_color = y->color;
        Node<ValType>* x = nullptr;

        if (!node->L) {
            x = node->R;
            _translate(node, node->R);
        } else if(!node->R) {
            x = node->L;
            _translate(node, node->L);
        } else {
            Node<ValType>* y = _minimum(node->R);
            old_y_color = y->color;
            x = y->R;
            if(y->P != node) {
                _translate(y, y->R);
                setR(y, node->R);
                setRP(y, y);
            }
            _translate(node, y);
            setL(y, node->L);
            setLP(y, y);
            setCLR(y, node->color);
        }

        ds_delete(node);

        if (NodeColor::NC_BLACK == old_y_color)
            _remove_fixup(x);
    }

private:
    void _insert_fixup(Node<ValType>* z)
    {
        //ֱ��z�ĸ��ڵ㲻Ϊ��ɫΪֹ
        while (z->P && isRED(z->P)) {
            if (isL(z->P)) {
                Node<ValType>* y = z->P->P->R;
                //1. z���ڵ���ֵܽڵ㲻���ڻ����Ǻ�ɫ��
                // ��ʱΪ�˱�֤���������4�����ƻ�����Ҫ�Ը��ڵ���常�ڵ�����Ⱦ�ɺ�ɫ��
                // ���������ڵ�Ⱦ�ɺ�ɫ��������ǰ�ڵ���Ϊ�����ڵ㣬����ʼ��һ�ֵ��޸�
                if ((y && isRED(y))) {
                    setBLK(z->P);
                    if(y)
                        setBLK(y);
                    setRED(z->P->P);
                    z = z->P->P;
                } else {
                    //2. z�Ǹ��ڵ�����ӽڵ�
                    if (isR(z)) {
                        z = z->P;
                        _rotateL(z);
                    } 
                    //3. 
                    if (z->P) {
                        setBLK(z->P);
                        if (z->P->P) {
                            setRED(z->P->P);
                            _rotateR(z->P->P);
                        }
                    }
                }
            } else {
                Node<ValType>* y = z->P->P->L;
                if ((y && isRED(y))) {
                    setBLK(z->P);
                    if (y)
                        setBLK(y);
                    setRED(z->P->P);
                    z = z->P->P;
                } else {
                    if (isL(z)) {
                        z = z->P;
                        _rotateR(z);
                    }
                    if (z->P) {
                        setBLK(z->P);
                        if (z->P->P) {
                            setRED(z->P->P);
                            _rotateL(z->P->P);
                        }
                    }
                }
            }
        }
        if (_root->P)
            _root = _root->P;
        setBLK(_root);
    }

    void _remove_fixup(Node<ValType>* z)
    {

    }

    void _rotateL(Node<ValType>* z)
    {
        Node<ValType>* w = z->R;
        
        if (w) {
            Node<ValType>* rl = w->L;
            setP(w, z->P);

            if (z->P) {
                if (isL(z))
                    setPL(z, w);
                else
                    setPR(z, w);
            }

            setP(z, w);
            setL(w, z);
            setR(z, rl);
            if(rl)
                setP(rl, z);
        }
    }

    void _rotateR(Node<ValType>* w)
    {
        Node<ValType>* z = w->L;
        
        if (w) {
            Node<ValType>* zr = z->R;
            
            if (w->P) {
                if (isL(w))
                    setPL(w, z);
                else
                    setPR(w, z);
            }

            setP(z, w->P);
            setP(w, z);
            setR(z, w);
            
            setL(w, zr);
            if(zr)
                setP(zr, w);
        }
    }
};

#endif