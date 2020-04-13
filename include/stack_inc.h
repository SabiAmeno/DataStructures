#ifndef _STACK_INC_H_
#define _STACK_INC_H_

#include <exception>

template<typename DataType>
class Stack
{
public:
    virtual ~Stack() {}

    virtual DataType top() = 0;
    virtual void push(const DataType& data) = 0;
    virtual void pop() = 0;
    
    virtual bool is_empty() const { return m_size == 0; }
    virtual int size() const { return m_size; }

protected:
    Stack() : m_size(0) {}

    int m_size;
};

template<typename DataType, unsigned int Size>
class SeqStack : public Stack<DataType>
{
public:
    SeqStack() {}

    virtual DataType top()
    {
        if (is_empty())
            throw std::exception("none data");

        return m_data[m_size - 1];
    }

    virtual void push(const DataType& data)
    {
        if (m_size >= Size)
            throw std::exception("out of space");

        m_data[m_size++] = data;
    }
    virtual void pop()
    {
        if (is_empty())
            throw std::exception("stack is empty");

        m_size--;
    }
private:
    int      m_size = 0;
    DataType m_data[Size] = { 0 };
};

/**
 * @brief 单链表实现的栈，如果去看c++标准，你会发现stack默认是使用deque去实现的
 *
 */
template<typename DataType>
class NSeqStack : public Stack<DataType>
{
    struct Node
    {
        DataType data;
        Node* next;
    };
public:
    NSeqStack() : m_root(nullptr) { }
    virtual ~NSeqStack() 
    {
        while (!is_empty())
        {
            pop();
        }
    }

    virtual DataType top() 
    {
        if (is_empty())
            throw std::exception("none data");

        return m_root->data;
    }
    virtual void push(const DataType& data)
    {
        Node* tmp = new Node();
        tmp->data = data;

        tmp->next = m_root;
        m_root = tmp;

        m_size++;
    }
    virtual void pop()
    {
        if(is_empty())
            throw std::exception("none data");

        Node* next = m_root->next;
        delete m_root;
        m_root = next;

        m_size--;
    }
private:
    Node* m_root;
};

#endif