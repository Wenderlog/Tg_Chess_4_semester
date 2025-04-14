//
// Created by Кирилл Грибанов  on 05/04/2025.
//

#include "Server_Manager.h"

idGenerator::idGenerator(int id) : id_(id) {}

int idGenerator::NextID() {
  return id_.fetch_add(1);
}
