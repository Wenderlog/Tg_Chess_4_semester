//
// Created by Кирилл Грибанов  on 05/04/2025.
//

#pragma once

#include <mutex>
#include <atomic>
#include <thread>
#include <unordered_map>
#include "My_Blocking_Queue.h"

class idGenerator {
    public:
   idGenerator(int id);
   int NextID();


  private:
    std::atomic<int> id_;
  };

class ThreadPool {

};

