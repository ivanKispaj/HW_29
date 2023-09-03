//
// FineGrainedQueue.cpp
// 2023-09-03
// Ivan Konishchev
//

#include "FineGrainedQueue.h"

FineGrainedQueue::Node::~Node()
{
    delete node_mutex;
    value = -99999999;
}

FineGrainedQueue::~FineGrainedQueue()
{
    queue_mutex->lock();
    Node *cur, *temp;
    cur = head;
    if (!cur)
    {
        queue_mutex->unlock();
        delete queue_mutex;
        return;
    }
    temp = cur;
    if (!(cur = cur->next))
    {
        delete temp;
        queue_mutex->unlock();
        delete queue_mutex;
        return;
    }

    cur->node_mutex->lock();
    temp->node_mutex->unlock();
    delete temp;
    queue_mutex->unlock();
    while (cur)
    {
        temp = cur;
        if ((cur = cur->next))
        {
            cur->node_mutex->lock();
            temp->node_mutex->unlock();
            delete temp;
        }
    }
    delete temp;
    delete queue_mutex;
}

void FineGrainedQueue::remove(const int &value)
{
    Node *prev, *cur; // указатели на предыдущий и текущий элемент
    queue_mutex->lock();
    if (!head)
    {
        queue_mutex->unlock();
        return;
    }
    prev = this->head; // старт: предыдущий = начало списка
    cur = prev->next;  // старт: текущий = второй элемент списка

    prev->node_mutex->lock();
    if (prev->value == value)
    {
        prev->node_mutex->unlock();
        delete prev;
        head = cur;
        queue_mutex->unlock();
        return;
    }

    if (!cur)
    {
        prev->node_mutex->unlock();
        queue_mutex->unlock();
        return;
    }

    cur->node_mutex->lock();
    queue_mutex->unlock();
    while (cur) // пока указатель на текущий элемент не нулевой...
    {
        if (cur->value == value) // если нашли элемент для удаления...
        {
            // то удаляем его
            prev->next = cur->next;
            prev->node_mutex->unlock();
            cur->node_mutex->unlock();
            delete cur;
            return;
        }

        // если не нашли элемент для удаления, то двигаемся дальше по очереди
        Node *old_prev = prev;
        prev = cur;
        cur = cur->next;

        old_prev->node_mutex->unlock(); // обратите внимание, после сдвига что анлочится "старый" предыдущий элемент
        if (!cur)
        {
            break;
        }
        cur->node_mutex->lock(); // а потом лочится новый текущий элемент
    }
    prev->node_mutex->unlock();
}

void FineGrainedQueue::remove(const int &&value)
{
    int numRemove = value;
    remove(numRemove);
}

void FineGrainedQueue::inserAt(int pos, const int &value)
{
    Node *cur, *next;
    Node *append = new Node;
    append->value = value;
    queue_mutex->lock();
    if (!head)
    {
        head = append;
        queue_mutex->unlock();
        return;
    }
    cur = this->head;
    next = cur->next;
    if (!next)
    {
        cur->next = append;
        queue_mutex->unlock();
        return;
    }

    if (pos == 0)
    {
        append->next = cur;
        head = append;
        queue_mutex->unlock();
        // cur->node_mutex->unlock();
        //  next->node_mutex->unlock();
        return;
    }

    cur->node_mutex->lock();
    next->node_mutex->lock();
    queue_mutex->unlock();

    int curIndex{1};
    while (next)
    {
        if (curIndex == pos)
        {
            break;
        }
        curIndex++;
        Node *old_cur = cur;
        cur = next;
        next = next->next;

        old_cur->node_mutex->unlock();
        if (!next)
        {
            break;
        }
        next->node_mutex->lock();
    }

    if (next)
    {
        append->next = next;
        next->node_mutex->unlock();
    }
    cur->next = append;
    cur->node_mutex->unlock();
}

void FineGrainedQueue::inserAt(int pos, const int &&value)
{
    int appendValue = value;
    inserAt(pos, appendValue);
}

