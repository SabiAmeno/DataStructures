#include <iostream>
#include "list_inc.h"
//#include "stack_inc.h"
#include <stack>
#include <cassert>
#include "btree_inc.h"
#include "rbtree_inc.h"
#include "queue_inc.h"

template<typename C, typename T>
void ContainerPrint(const C& c)
{
    int size = c.size();

    for (int i = 0; i < size; ++i) {
        T ret = c.get(i);
        std::cout << ret << " ";
    }

    std::cout << std::endl;
}

void testList()
{
    NSeqList<int> nseq_list;
    nseq_list.insert(0, 5);
    nseq_list.insert(1, 4);
    nseq_list.insert(2, 3);
    nseq_list.insert(3, 2);
    nseq_list.insert(4, 1);

    ContainerPrint<NSeqList<int>, int>(nseq_list);

    nseq_list.push_back(9);
    nseq_list.remove(3);

    ContainerPrint<NSeqList<int>, int>(nseq_list);
    nseq_list.clear();

    DNSeqList<int> dnseq_list;
    dnseq_list.insert(0, 5);
    dnseq_list.insert(1, 4);
    dnseq_list.insert(2, 3);
    dnseq_list.insert(3, 2);
    dnseq_list.insert(4, 1);
    ContainerPrint<DNSeqList<int>, int>(dnseq_list);
}

void testStack()
{
    Stack<int> stack;
    stack.push(5);
    stack.push(6);
    stack.push(3);
    stack.push(7);
    stack.push(2);

    try
    {
        stack.push(6);
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    while (!stack.is_empty())
    {
        std::cout << stack.top() << " ";
        stack.pop();
    }

    std::cout << std::endl;

    try
    {
        stack.pop();
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void testQueue()
{
    Queue<int> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);
    queue.enqueue(4);
    queue.enqueue(5);
    queue.enqueue(6);

    assert(queue.front() == 1);

    assert(queue.back() == 6);
    
    queue.dequeue();

    assert(queue.size() == 5);

    queue.enqueue(19);

    while (!queue.is_empty()) {
        std::cout << queue.front() << " ";
        queue.dequeue();
    }
    std::cout << std::endl;
    
    assert(queue.size() == 0);

    assert(queue.is_empty());
}

template<typename Type, typename T>
void treePrint(const T& tree)
{
    if (tree.is_empty())
        return;

    T::NS* node = nullptr;
    T::Nodes nodes;
    Queue<T::NS*> deffered;

    tree.children(nodes, node);
    deffered.enqueue(nodes.find(0)->data);
    deffered.enqueue(0);
    std::cout << nodes.find(0)->data->data << std::endl;

    while (!deffered.is_empty()) {
        T::NS* topn = deffered.front();
        deffered.dequeue();
        if (!topn) {
            std::cout << std::endl;
            continue;
        }
        tree.children(nodes, topn);

        if (!nodes.is_empty()) {
            for (int i = 0; i < nodes.size(); ++i) {
                deffered.enqueue(nodes.find(i)->data);
                std::cout << nodes.find(i)->data->data << " ";
            }
            deffered.enqueue(nullptr);
        }
    }
}

void treeTest()
{
    BinST<int> btree;
    btree.insert(12);
    btree.insert(7);
    btree.insert(4);
    btree.insert(44);
    btree.insert(11);
    btree.insert(34);
    btree.insert(45);
    btree.insert(1);

    btree.remove(12);
    btree.remove(7);
    btree.remove(4);
    btree.remove(44);
    btree.remove(11);
    btree.remove(34);
    btree.remove(45);
    btree.remove(1);

    RedBTree<int> rbtree;
    int vals[] = { 11, 2, 14, 15, 1, 7, 5, 4, 8, 19, 12, 18, 33 };
    for (int i = 0; i < 13; ++i)
        rbtree.insert(vals[i]);

    treePrint<int, RedBTree<int>>(rbtree);

    for (int i = 0; i < 13; ++i) {
        rbtree.remove(vals[i]);
    }
}

int main()
{
    treeTest();

    return 0;
}