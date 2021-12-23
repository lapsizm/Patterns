// Copyright 2021 Kirill Murygin <murgyin_280702@mail.ru>

#include "Histogram.hpp"

Histogram::Histogram(PageContainer &page) { page.Attach(this); }

void Histogram::OnDataLoaded([[maybe_unused]]const std::vector<Item> &old_items,
                             const std::vector<Item> &new_items) {
  for (auto &item : new_items) {
    avg += item.score;
  }
  avg /= new_items.size();
  avg_vec.push_back(avg);
  count_vec.push_back(count);
  Log::GetInstance()->Write(std::to_string(avg) + " is average score");
  Log::GetInstance()->WriteDebug(std::to_string(count) + " times skipped");
  count = 0;
  avg = 0;
}

void Histogram::OnSkipped([[maybe_unused]]const Item &item) { ++count; }
