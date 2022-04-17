// Copyright 2022 Petrova Kseniya <ksyushki5@yandex.ru>
#include "client.hpp"
int main() {
  Client cl;
  // {"input":"hel"}
  cl.request();
  std::cout << std::endl;
  std::cout << "RESPONSE: " << std::endl;
  std::cout << std::endl;
  cl.start();
}