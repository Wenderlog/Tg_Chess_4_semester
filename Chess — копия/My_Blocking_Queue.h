//
// Created by Кирилл Грибанов  on 05/04/2025.
//

#pragma once
#include <iostream>
#include <optional>
#include <queue>


template <typename T>
class BlockingQueue {
  public:
    std::optional<T> Get(); // перенести сюда реализацию шаблонов, удалить cpp
    void ShutDown();
    void Close();
    void PushBack(T object);
  private:
      std::queue<T> queue_;
      bool isOpen_ = true;
      std::mutex mutex_;
      std::condition_variable cv_;
      size_t count_ = 0;

};

template <typename T>
void BlockingQueue<T>::PushBack(T object) {
  std::lock_guard lock(mutex_); // wait переписатьь через цикл с использованием cv_
  if (isOpen_) {
    queue_.push_back(std::move(object));
    if (0 < count_) {
      cv_.notify_one();
    }
  }
}

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

template<typename T>
void BlockingQueue<T>::ShutDown() {
  std::lock_guard lock(mutex_);
  while (!queue_.empty()) {
    queue_.clear();
    if (count_ == 1) {
      cv_.notify_one();
    }
    if (1 < count_) {
      cv_.notify_all();
    }
  }
}

template<typename T>
std::optional<T> BlockingQueue<T>::Get() {
  std::unique_lock lock(mutex_);

  if (!queue_.empty()) {
    T object = queue_.front();
    queue_.pop();
    cv_.notify_one();
    return object;
  }

    std::cerr << "Очередь пустая" << std::endl;
    ++count_;
    cv_.wait(lock, [&](){return !queue_.empty() || !isOpen_;}); // добавить второй cv_ на empty.
    --count_;

    if (!queue_.empty()) {
      T object = queue_.front();
      queue_.pop();
      return object;
    }

  return std::nullopt;
}


