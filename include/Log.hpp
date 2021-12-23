// Copyright 2021 Kirill Murygin <murgyin_280702@mail.ru>

#ifndef INCLUDE_LOG_HPP_
#define INCLUDE_LOG_HPP_

#include <iostream>
#include <fstream>
#include <string_view>
#include <vector>
#include <string>
#include <memory>

struct Item {
  std::string id;
  std::string name;
  float score = 0;
};


class Log {
 public:
  void Write(std::string_view message) const;

  void WriteDebug(std::string_view message) const;

  //альтернативный конструктор, возвращает один и тот же синглтон
  static Log* GetInstance();



 private:
  size_t level_ = 0;
  mutable std::ostream* out_;
  //синглтон должен быть скрыт от пользователя для создания
  Log() : level_(0), out_(&std::cout) {}
  explicit Log(size_t lvl) : level_(lvl), out_(&std::cout) {}
  //указатель на синглтон, уник птр - некопируемый
  static std::unique_ptr<Log> log_;
};







#endif // INCLUDE_LOG_HPP_
