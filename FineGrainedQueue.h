//
// FineGrainedQueue.h
// 2023-09-02
// Ivan Konishchev
//

#pragma once

#include <mutex>
#include <iostream>

class FineGrainedQueue
{
    struct Node
    {
        /// @brief init
        Node() : next(nullptr), node_mutex(new std::mutex), value(-99999999) {}

        /// @brief Deinit
        ~Node();

        /// @brief Delete Copy Constructor
        /// @param node
        Node(const Node &node) = delete;

        /// @brief Delete assignment operator
        /// @param node
        /// @return
        Node &operator=(const Node &node) = delete;

        // Property
        Node *next{nullptr};
        std::mutex *node_mutex;
        int value;
    };

    // Private property
    Node *head{nullptr};
    std::mutex *queue_mutex;

public:
    /// @brief init
    FineGrainedQueue() : head(nullptr), queue_mutex(new std::mutex) {}

    /// @brief Delete Copy Constructor
    /// @param queue
    FineGrainedQueue(const FineGrainedQueue &queue) = delete;

    /// @brief Delete assignment operator
    /// @param queue
    /// @return
    FineGrainedQueue &operator=(const FineGrainedQueue &queue) = delete;

    /// @brief Deinit
    ~FineGrainedQueue();

    /// @brief Remove vakue from queue for left value
    /// @param value 
    void remove(const int &value);
    
    /// @brief Remove vakue from queue for right value
    /// @param value 
    void remove(const int &&value);
    
    /// @brief Insert value to queue for left value
    /// @param pos 
    /// @param value 
    void inserAt(int pos, const int &value);
    
    /// @brief Insert value to queue for right value;
    /// @param pos position where to insert
    /// @param value the value to insert
    void inserAt(int pos, const int &&value);
    

    friend std::ostream &operator<<(std::ostream &out, const FineGrainedQueue &queue)
    {
        queue.queue_mutex->lock();
        if (!queue.head)
        {
            queue.queue_mutex->unlock();
            return out;
        }
        Node *cur = queue.head;
        out << "queue: " << cur->value;
        Node *next;
        if (!(next = cur->next))
        {
            queue.queue_mutex->unlock();
            return out;
        }

        next->node_mutex->lock();
        queue.queue_mutex->unlock();
        while (next)
        {
            out << " -> " << next->value;
            Node *temp = next;
            if ((next = next->next))
            {
                next->node_mutex->lock();
            }
            temp->node_mutex->unlock();
        }
        out << "\n";
        return out;
    }
};
