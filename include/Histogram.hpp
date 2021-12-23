// Copyright 2021 Kirill Murygin <murgyin_280702@mail.ru>

#ifndef INCLUDE_HISTOGRAM_HPP_
#define INCLUDE_HISTOGRAM_HPP_

#include "Observer.hpp"
#include "PageContainer.hpp"
#include <string>
#include <vector>

class Histogram : public Observer {
 private:
  std::vector<float> avg_vec;
  std::vector<size_t> count_vec;
  size_t count = 0;
  float avg = 0;

 public:
  Histogram() = default;
  explicit Histogram(PageContainer& page);

  void OnDataLoaded(const std::vector<Item>& old_items,
                    const std::vector<Item>& new_items) override;

  void OnSkipped(const Item& item) override;

  void OnRawDataLoaded(
      [[maybe_unused]] const std::vector<std::string>& old_items,
      [[maybe_unused]] const std::vector<std::string>& new_items) override {}

  std::vector<size_t> GetCountVec() { return count_vec; }
  std::vector<float> GetAvgVec() { return avg_vec; }
};

#endif  // INCLUDE_HISTOGRAM_HPP_
