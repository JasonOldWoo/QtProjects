#pragma once
#include <iostream>

template <class T>
class QueueNode
{
public:
    T element;
    QueueNode<T> *next;  
};

template <class T>
class LinkQueue
{
public:

    LinkQueue(void)
    {
        front = new QueueNode<T>();
        rear = new QueueNode<T>();
        front->next = NULL;
        rear->next = front;
    }

    void enque(T obj)
    {
        QueueNode<T> *newNode = new QueueNode<T>();
        newNode->element = obj;
        newNode->next = NULL;
        rear->next->next = newNode;
        rear->next = newNode;
    }

    T deque()
    {
        if(!IsEmpty())
        {
            QueueNode<T> *topNode = front->next;
            
            if(topNode->next==NULL) 
            {
                //此时topNode为最后一个节点,front和rear的next都指向它,取出topNode后队列变为空,链队变回到初始状态

                front->next = NULL;
                rear->next = front;
            }
            else
                front->next = topNode->next;

            T temp = topNode->element;

            delete(topNode);

            return temp;
        }
        else
            return NULL;
    }

    bool IsEmpty()
    {
        if(front->next==NULL)
            return true;
        else
            return false;
    }

    ~LinkQueue(void)
    {
    }

private:
    QueueNode<T> *front;
    QueueNode<T> *rear;
};
