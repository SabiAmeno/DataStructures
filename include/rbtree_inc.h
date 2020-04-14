#ifndef _RBTREE_INC_H_
#define _RBTREE_INC_H_

#include "btree_inc.h"

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

#define isRED(z) (NodeColor::NC_RED == (z)->color)
#define isBLK(z) (NodeColor::NC_BLACK == (z)->color)
#define setRED(z) (z)->color = NodeColor::NC_RED
#define setBLK(z) (z)->color = NodeColor::NC_BLACK

template<typename ValType, template<typename T> typename Node = RBNode>
class RedBTree : public BinST<ValType, Node>
{
public:
    virtual void insert(const ValType& val)
    {
        Node<ValType>* nnode = _insert(val);

        _insert_fixup(nnode);
    }

    virtual void remove(Node<ValType>* node)
    {
        
    }

private:
    void _insert_fixup(Node<ValType>* z)
    {
        //直到z的父节点不为红色为止
        while (z->P && isRED(z->P)) {
            Node<ValType>* p = z->P;
            Node<ValType>* pp = p->P;

            if (p == pp->L) {
                //1. z父节点的兄弟节点不存在或者是红色的
                if ((pp->R && isRED(pp->R)) || !pp->R) {
                    setBLK(p);
                    if(pp->R)
                        setBLK(pp->R);
                    setRED(pp);
                    z = pp;
                } else {
                    //2. z是父节点的右子节点
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
                if ((pp->L && isRED(pp->L)) || !pp->L) {
                    setBLK(p);
                    if(pp->L)
                        setBLK(pp->L);
                    setRED(pp);
                    z = pp;
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
        while (_root->P)
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