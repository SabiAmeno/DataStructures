#ifndef _LIST_INC_H
#define _LIST_INC_H

#include <iostream>
#include <cstring>

template <typename DataType>
class List
{
public:
    using DataTypeConstRef = const DataType&;
public:
    virtual bool insert(int pos, DataTypeConstRef ref) = 0;
    virtual bool push_back(DataTypeConstRef ref) = 0;
    virtual bool remove(int pos) = 0;

    virtual DataType get(int pos) const = 0;
    virtual int size() const = 0;
    virtual void clear() = 0;
};

template<typename DataType>
class SeqList : public List<DataType>
{
public:
    SeqList(int capacity)
        : m_capacity(capacity), m_size(0)
    {
        m_data = new DataType[m_capacity];
        memset(m_data, 0, m_capacity * sizeof(DataType));
    }

    virtual ~SeqList()
    {
        if (m_data) {
            delete[] m_data;
        }
    }

    bool insert(int pos, typename List<DataType>::DataTypeConstRef ref)
    {
        if (m_size + 1 > m_capacity)
            return false;

        for (int i = m_size; i > pos; --i) {
            m_data[i + 1] = m_data[i];
        }

        m_size++;
        m_data[pos] = ref;

        return true;
    }

    bool push_back(typename List<DataType>::DataTypeConstRef ref) {
        return insert(m_size, ref);
    }

    bool remove(int pos) {
        if (pos >= m_size)
            return false;

        for (int i = pos; i < m_size; ++i) {
            m_data[i] = m_data[i + 1];
        }

        m_size--;

        return true;
    }

    DataType get(int pos) const {
        if (pos < 0 || pos >= m_size)
            return DataType();

        return m_data[pos];
    }

    int size() const {
        return m_size;
    }

    void clear() {
        m_size = 0;
    }

    bool is_empty() const {
        return m_size == 0;
    }
private:
    int    m_size;
    int    m_capacity;
    DataType* m_data;
};

template<typename DataType>
struct LNode
{
    DataType data;

    LNode* next;
};

template <typename DataType, template<typename T> typename Node = LNode>
class NSeqList : public List<DataType>
{
public:
    NSeqList()
        : m_size(0)
    {
        m_head = new Node<DataType>();
    }

    virtual ~NSeqList()
    {
        clear();
    }

    virtual bool insert(int pos, typename List<DataType>::DataTypeConstRef ref)
    {
        Node<DataType>* tmp = find(pos - 1);
        if (!tmp)
            return false;

        Node<DataType>* inode = new Node<DataType>();
        inode->data = ref;
        inode->next = tmp->next;
        tmp->next = inode;

        m_size++;

        return true;
    }

    virtual bool push_back(typename List<DataType>::DataTypeConstRef ref) {
        return insert(m_size, ref);
    }

    virtual bool remove(int pos) {
        Node<DataType>* pre = find(pos - 1);

        if (!pre || !pre->next)
            return false;

        Node<DataType>* next = pre->next;
        Node<DataType>* next2 = next->next;
        pre->next = next2;
        delete next;

        m_size--;

        return true;
    }

    DataType get(int pos) const {
        Node<DataType>* node = find(pos);
        if (!node)
            throw std::out_of_range("Illegal parameter 'pos'");

        return node->data;
    }

    int size() const {
        return m_size;
    }

    void clear() {
        Node<DataType>* next = m_head->next;

        while (next) {
            Node<DataType>* tmp = next->next;
            delete next;
            next = tmp;
        }
        m_size = 0;
        m_head->next = nullptr;
    }

    virtual Node<DataType>* find(int pos) const {
        if (pos < -1 || pos >= m_size)
            return nullptr;
        if (pos == -1)
            return m_head;
        Node<DataType>* tmp = m_head;

        while (pos-- >= 0 && tmp->next)
            tmp = tmp->next;

        if (pos > 0)
            return nullptr;

        return tmp;
    }

    bool is_empty() const {
        return m_size == 0;
    }
protected:
    int             m_size;
    Node<DataType>* m_head;
};

template<typename DataType>
struct DLNode
{
    DataType data;

    DLNode* next;
    DLNode* pre;
};

template<typename DataType, template<typename T> typename Node = DLNode>
class DNSeqList : public NSeqList<DataType, Node>
{
public:
    DNSeqList() : NSeqList()
    {

    }

    virtual bool insert(int pos, DataTypeConstRef ref)
    {
        Node<DataType>* pre = find(pos - 1);
        if (!pre)
            return false;

        Node<DataType>* newNode = new Node<DataType>();
        newNode->data = ref;
        newNode->next = pre->next;
        newNode->pre = pre;
        if (pre->next)
            pre->next->pre = newNode;
        pre->next = newNode;

        m_size++;

        return true;
    }
    virtual bool remove(int pos)
    {
        Node<DataType>* pre = find(pos - 1);
        if (!pre || !pre->next)
            return false;

        Node<DataType>* currNode = pre->next;
        if (currNode->next)
            currNode->next->pre = pre;
        pre->next = currNode->next;
        delete currNode;

        m_size--;

        return true;
    }
};

#endif
