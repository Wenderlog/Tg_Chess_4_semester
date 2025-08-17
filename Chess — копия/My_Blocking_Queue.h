//
// Created by Кирилл Грибанов  on 05/04/2025.
//
#pragma once

#include <condition_variable>
#include <iostream>
#include <optional>
#include <queue>

/*! \brief
 *   Thread-safe blocking queue template.
 *
 *   Supports multiple producers and consumers. Items can be pushed back,
 *   retrieved, and the queue can be closed or shut down.
 *
 *   \tparam T Type of elements stored in the queue.
 */
template <typename T>
class BlockingQueue {
public:
    /*! \brief Retrieves an item from the queue.
     *
     *   If the queue is empty, waits until an item is available or the queue is closed.
     *
     *   \return std::optional containing the retrieved item, or std::nullopt if closed and empty.
     */
    std::optional<T> Get();

    /*! \brief Closes the queue.
     *
     *   No more items can be added after closing. Notifies all waiting threads.
     */
    void Close();

    /*! \brief Shuts down the queue.
     *
     *   Clears all items and notifies waiting threads.
     */
    void ShutDown();

    /*! \brief Adds an item to the back of the queue.
     *
     *   \param object Item to add.
     */
    void PushBack(T object);

private:
    std::queue<T> queue_;                 ///< Underlying container for storing items.
    bool isOpen_ = true;                  ///< Indicates if the queue is open for adding items.
    std::mutex mutex_;                    ///< Mutex to synchronize access.
    std::condition_variable cv_;          ///< Condition variable for blocking/waiting.
    size_t count_ = 0;                    ///< Number of threads waiting for items.
};

/*! \brief Adds an item to the queue if it is open. */
template <typename T>
void BlockingQueue<T>::PushBack(T object) {
    std::lock_guard lock(mutex_);
    if (isOpen_) {
        queue_.push(std::move(object));
        if (0 < count_) {
            cv_.notify_one();
        }
    }
}

/*! \brief Closes the queue and notifies waiting threads. */
template<typename T>
void BlockingQueue<T>::Close() {
    std::lock_guard lock(mutex_);
    if (isOpen_) {
        isOpen_ = false;
        if (count_ == 1) {
            cv_.notify_one();
        }
        if (1 < count_) {
            cv_.notify_all();
        }
    }
}

/*! \brief Clears the queue and notifies waiting threads. */
template<typename T>
void BlockingQueue<T>::ShutDown() {
    std::lock_guard lock(mutex_);
    while (!queue_.empty()) {
        queue_.pop();
        if (count_ == 1) {
            cv_.notify_one();
        }
        if (1 < count_) {
            cv_.notify_all();
        }
    }
}

/*! \brief Retrieves an item from the queue, blocking if empty. */
template<typename T>
std::optional<T> BlockingQueue<T>::Get() {
    std::unique_lock lock(mutex_);

    if (!queue_.empty()) {
        T object = queue_.front();
        queue_.pop();
        cv_.notify_one();
        return object;
    }

    std::cerr << "Queue is empty" << std::endl;
    ++count_;
    cv_.wait(lock, [&](){ return !queue_.empty() || !isOpen_; });
    --count_;

    if (!queue_.empty()) {
        T object = queue_.front();
        queue_.pop();
        return object;
    }

    return std::nullopt;
}


