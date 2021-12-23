// Copyright 2021 Kirill Murygin <murgyin_280702@mail.ru>

#ifndef INCLUDE_STATSENDER_HPP_
#define INCLUDE_STATSENDER_HPP_

#include "Observer.hpp"
#include "PageContainer.hpp"
#include <string>
#include <vector>

class StatSender : Observer {
 private:
  virtual void AsyncSend(const std::vector<Item>& items, std::string_view path);
  std::ofstream fstr{"network", std::ios::binary};

 public:
  StatSender() = default;
  explicit StatSender(PageContainer& page);

  void OnDataLoaded(const std::vector<Item>& old_items,
                    const std::vector<Item>& new_items) override;
  void OnSkipped(const Item& item) override;

  void OnRawDataLoaded(
      [[maybe_unused]] const std::vector<std::string>& old_items,
      [[maybe_unused]] const std::vector<std::string>& new_items) override {}

};

#endif  // INCLUDE_STATSENDER_HPP_
