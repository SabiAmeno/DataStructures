#ifndef _LIST_INCLUDE_H
#define _LIST_INCLUDE_H

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
        if(m_data) {
            delete [] m_data;
        }
    }

    bool insert(int pos, typename List<DataType>::DataTypeConstRef ref)
    {
        if(m_size + 1 > m_capacity)
            return false;

        for (int i = m_size;i > pos;--i) {
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
        if(pos >= m_size)
            return false;

        for (int i = pos;i < m_size;++i) {
            m_data[i] = m_data[i + 1];
        }

        m_size--;

        return true;
    }

    DataType get(int pos) const{
        if(pos < 0 || pos >= m_size)
            return DataType();

        return m_data[pos];
    }

    int size() const{
        return m_size;
    }

    void clear() {
        m_size = 0;
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

template <typename DataType>
class NSeqList : public List<DataType>
{
public:
    NSeqList()
    {
        m_head = new LNode<DataType>();
    }

    virtual ~NSeqList()
    {
        clear();
    }

    virtual bool insert(int pos, typename List<DataType>::DataTypeConstRef ref)
    {
        LNode<DataType>* tmp = _get(pos - 1);
        if(!tmp)
            return false;

        LNode<DataType>* inode = new LNode<DataType>();
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
        LNode<DataType>* pre = _get(pos - 1);

        if(!pre || !pre->next)
            return false;

        LNode<DataType>* next = pre->next;
        LNode<DataType>* next2 = next->next;
        pre->next = next2;
        delete next;

        m_size--;

        return true;
    }

    DataType get(int pos) const{
        LNode<DataType>* node = _get(pos);
        if(!node)
            throw std::out_of_range("Illegal parameter 'pos'");

        return node->data;
    }

    int size() const{
        return m_size;
    }

    void clear() {
        LNode<DataType>* tmp = m_head;

        while (tmp->next) {
            LNode<DataType>* next = tmp->next;
            delete tmp;
            tmp = next;
        }
        m_size = 0;
        m_head->next = nullptr;
    }
private:
    LNode<DataType>* _get(int pos) const {
        if(pos < -1 || pos >= m_size)
            return nullptr;
        if(pos == -1)
            return m_head;
        LNode<DataType>* tmp = m_head;

        while(pos-- >= 0 && tmp->next)
            tmp = tmp->next;

        if(pos > 0 )
            return nullptr;

        return tmp;
    }
private:
    int              m_size;
    LNode<DataType>* m_head;
};
#endif
