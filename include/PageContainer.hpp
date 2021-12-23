// Copyright 2021 Kirill Murygin <murgyin_280702@mail.ru>

#ifndef INCLUDE_PAGECONTAINER_HPP_
#define INCLUDE_PAGECONTAINER_HPP_
#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "Observer.hpp"

class PageContainer : public ItemSubject {
 private:
  void NotifyOnDataLoaded(const std::vector<Item>& new_items) override;

  void NotifyOnRawDataLoaded(
      const std::vector<std::string>& new_items) override;

  void NotifyOnSkipped(const Item& item) override;

  std::vector<Item> data_;
  std::vector<std::string> raw_data_;
  std::vector<Observer*> observers;

 public:
  PageContainer() = default;

  void Load(std::istream& io, float threshold);

  const Item& ByIndex(size_t i) const;
  const Item& ById(const std::string& id) const;

  void Reload(float threshold);

  void Attach(Observer* obs) override;

  void Detach(Observer* obs) override;

  std::vector<Item> GetData() { return data_; }
  std::vector<std::string> GetRawData() { return raw_data_; }

};

#endif  // INCLUDE_PAGECONTAINER_HPP_
