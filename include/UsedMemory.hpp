// Copyright 2021 Kirill Murygin <murgyin_280702@mail.ru>

#ifndef INCLUDE_USEDMEMORY_HPP_
#define INCLUDE_USEDMEMORY_HPP_

#include "Observer.hpp"
#include "PageContainer.hpp"
#include <string>
#include <vector>

class UsedMemory : Observer{
 private:
  size_t used_ = 0;

 public:
  UsedMemory() = default;
  explicit UsedMemory(PageContainer& page);

  void OnDataLoaded(const std::vector<Item>& old_items,
                  const std::vector<Item>& new_items) override;
  void OnRawDataLoaded(const std::vector<std::string>& old_items,
                     const std::vector<std::string>& new_items) override;

  void OnSkipped([[maybe_unused]]const Item& item) override {}

  size_t used() const;

};


#endif // INCLUDE_USEDMEMORY_HPP_
