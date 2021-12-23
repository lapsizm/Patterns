// Copyright 2021 Kirill Murygin <murgyin_280702@mail.ru>

#include "Log.hpp"

std::unique_ptr<Log> Log::log_ = nullptr;

void Log::Write(std::string_view message) const {
  *out_ << message << std::endl;
}

void Log::WriteDebug(std::string_view message) const {
  if (level_ > 0) *out_ << message << std::endl;
}

Log* Log::GetInstance() {
  if (log_ == nullptr) {
    log_ = std::unique_ptr<Log>(new Log);
  }
  return log_.get();
}
