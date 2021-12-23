// Copyright 2021 Kirill Murygin <murgyin_280702@mail.ru>

#ifndef INCLUDE_OBSERVER_HPP_
#define INCLUDE_OBSERVER_HPP_

#include <string>
#include <vector>


#include "Log.hpp"

/* класс наблюдателя - от него будут наследоваться StatSender, UsedMemory,
   Histogram. Виртуальные функции будут переопределяться динамически у наших
   классов*/

class Observer {
 public:
  virtual void OnSkipped(const Item& item) = 0;

  virtual void OnDataLoaded(const std::vector<Item>& old_items,
                            const std::vector<Item>& new_items) = 0;

  virtual void OnRawDataLoaded(const std::vector<std::string>& old_items,
                               const std::vector<std::string>& new_items) = 0;

  //обязательно виртуальный деструктор
  virtual ~Observer() = default;
};

/* класс уведомителя - от него будет наследоваться PageContainer*/
class ItemSubject {
 public:
  virtual void Attach(Observer* obs) = 0;

  [[maybe_unused]] virtual void Detach(Observer* obs) = 0;

  virtual void NotifyOnDataLoaded(const std::vector<Item>& new_items) = 0;

  virtual void NotifyOnRawDataLoaded(
      const std::vector<std::string>& new_items) = 0;

  virtual void NotifyOnSkipped(const Item& item) = 0;

  virtual ~ItemSubject() = default;
};
#endif  // INCLUDE_OBSERVER_HPP_
