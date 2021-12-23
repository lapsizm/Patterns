// Copyright 2021 Kirill Murygin <murgyin_280702@mail.ru>

#include "StatSender.hpp"

StatSender::StatSender(PageContainer& page) { page.Attach(this); }

void StatSender::OnDataLoaded(
    [[maybe_unused]] const std::vector<Item>& old_items,
    const std::vector<Item>& new_items) {
  Log::GetInstance()->WriteDebug("StatSender::OnDataLoad");
  AsyncSend(new_items, "/items/loaded");
}

void StatSender::OnSkipped(const Item& item) {
  AsyncSend({item}, "/items/skipped");
}

void StatSender::AsyncSend(const std::vector<Item>& items,
                           std::string_view path) {
  Log::GetInstance()->Write(path);
  Log::GetInstance()->Write("send stat " + std::to_string(items.size()));

  for (const auto& item : items) {
    Log::GetInstance()->WriteDebug("send: " + item.id);
    // ... some code
    fstr << item.id << item.name << item.score;
    fstr.flush();
  }
}
