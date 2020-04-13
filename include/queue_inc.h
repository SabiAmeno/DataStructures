#ifndef _QUEUE_INC_H_
#define _QUEUE_INC_H_

#include <exception>

template<typename T>
class Deque
{
    struct Node
    {
        T data;

        Node* next;
        Node* prior;
    };
public:
    Deque() : _size(0) { _front = _rear = nullptr; }

    ~Deque()
    {
        while (!is_empty())
        {
            pop_front();
        }
    }

    void push_back(const T& data)
    {
        Node* tmp = new Node();
        tmp->data = data;

        if (!_front || !_rear)
            _front = _rear = tmp;
        else
        {
            tmp->next = _rear->next;
            tmp->prior = _rear;
            _rear->next = tmp;
            _rear = tmp;
        }

        _size++;
    }
    void push_front(const T& data)
    {
        Node* tmp = new Node();
        tmp->data = data;

        if (!_front || !_rear)
            _front = _rear = tmp;
        else
        {
            tmp->next = _front;
            _front->prior = tmp;
            _front = tmp;
        }

        _size++;
    }

    bool is_empty() const
    {
        return size() == 0;
    }

    T front() const
    {
        if (is_empty())
            throw std::exception("empty deque");

        return _front->data;
    }

    T back() const
    {
        if (is_empty())
            throw std::exception("empty deque");

        return _rear->data;
    }

    T pop_front()
    {
        if (is_empty())
            throw std::exception("empty deque");

        Node* next = _front->next;
        T val = _front->data;
        delete _front;
        _front = next;

        _size--;

        return val;
    }

    T pop_back() 
    {
        if(is_empty())
            throw std::exception("empty deque");

        Node* prior = _rear->prior;
        T val = _rear->data;
        delete _rear;
        _rear = prior;

        _size--;

        return val;
    }

    int size() const
    {
        return _size;
    }
private:
    
private:
    Node* _front;
    Node* _rear;

    int _size;
};

template<typename DataType, typename Container = Deque<DataType>>
class Queue
{
public:
    Queue() = default;

    void enqueue(const DataType& data)
    {
        _container.push_back(data);
    }

    void dequeue()
    {
        _container.pop_front();
    }

    DataType front() const
    {
        return _container.front();
    }

    DataType back() const
    {
        return _container.back();
    }

    bool is_empty() const
    {
        return _container.is_empty();
    }

    int size() const
    {
        return _container.size();
    }
private:
    Container _container;
};


template<typename DataType, typename Container = Deque<DataType>>
class Stack
{
public:
    Stack() = default;

    DataType top()
    {
        return _container.back();
    }
    void push(const DataType& data)
    {
        _container.push_back(data);
    }

    void pop()
    {
        _container.pop_back();
    }

    bool is_empty() const
    {
        return _container.is_empty();
    }
    int size() const
    {
        return _container.size();
    }

private:
    Container _container;
};

#endif