// Copyright 2021 Kirill Murygin <murgyin_280702@mail.ru>

#include "PageContainer.hpp"

#include <algorithm>
#include <set>
#include <sstream>

constexpr size_t kMinLines = 10;


void PageContainer::Load(std::istream& io, float threshold) {
  std::vector<std::string> raw_data;

  while (!io.eof()) {
    std::string line;
    std::getline(io, line, '\n');
    raw_data.push_back(std::move(line));
  }

  if (raw_data.size() < kMinLines) {
    throw std::runtime_error("too small input stream");
  }

  std::vector<Item> data;
  std::set<std::string> ids;
  for (const auto& line : raw_data) {
    std::stringstream stream(line);

    Item item;
    stream >> item.id >> item.name >> item.score;

    if (auto&& [_, inserted] = ids.insert(item.id); !inserted) {
      throw std::runtime_error("already seen");
    }

    if (item.score > threshold) {
      data.push_back(std::move(item));
    } else {
      NotifyOnSkipped(item);
    }
  }

  if (data.size() < kMinLines) {
    throw std::runtime_error("oops");
  }
  NotifyOnRawDataLoaded(raw_data);
  raw_data_ = std::move(raw_data);

  NotifyOnDataLoaded(data);
  data_ = std::move(data);
}

const Item& PageContainer::ByIndex(size_t i) const { return data_[i]; }

const Item& PageContainer::ById(const std::string& id) const {
  auto it = std::find_if(std::begin(data_), std::end(data_),
                         [&id](const auto& i) { return id == i.id; });
  return *it;
}

void PageContainer::Reload(float threshold) {
  std::vector<Item> data;
  std::set<std::string> ids;
  for (const auto& line : raw_data_) {
    std::stringstream stream(line);

    Item item;
    stream >> item.id >> item.name >> item.score;

    if (auto&& [_, inserted] = ids.insert(item.id); !inserted) {
      throw std::runtime_error("already seen");
    }

    if (item.score > threshold) {
      data.push_back(std::move(item));
    } else {
      NotifyOnSkipped(item);
    }
  }

  if (data.size() < kMinLines) {
    throw std::runtime_error("oops");
  }
  NotifyOnDataLoaded(data);
  data_ = std::move(data);
}

void PageContainer::NotifyOnDataLoaded(const std::vector<Item>& new_items) {
  for (auto& observer : observers) {
    observer->OnDataLoaded(data_, new_items);
  }
}

void PageContainer::NotifyOnRawDataLoaded(
    const std::vector<std::string>& new_items) {
  for (auto& observer : observers) {
    observer->OnRawDataLoaded(raw_data_, new_items);
  }
}

void PageContainer::NotifyOnSkipped(const Item& item) {
  for (auto& observer : observers) {
    observer->OnSkipped(item);
  }
}

void PageContainer::Attach(Observer* obs) { observers.push_back(obs); }

void PageContainer::Detach(Observer* obs) {
  observers.erase(std::find(observers.begin(), observers.end(), obs));
}
