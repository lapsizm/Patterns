// Copyright 2021 Your Name <your_email>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Histogram.hpp"
#include "Log.hpp"
#include "PageContainer.hpp"
#include "StatSender.hpp"
#include "UsedMemory.hpp"

TEST(PageContainerTest, LoadStream_input) {
  std::stringstream ss;

  /* size < kMin */
  ss << "1 Andrey 21\n2 Kirill 13\n3 Car 14\n5 Table 3";

  PageContainer page;
  UsedMemory memory(page);
  StatSender sender(page);
  Histogram histogram(page);

  EXPECT_THROW(page.Load(ss, 0.1), std::runtime_error);

  EXPECT_EQ(page.GetRawData().size(), 0);
  EXPECT_EQ(page.GetData().size(), 0);
  EXPECT_EQ(memory.used(), 0);
  EXPECT_EQ(histogram.GetCountVec().empty(), true);
}

TEST(PageContainerTest, LoadStream_seen) {
  std::stringstream ss;

  ss << "1 @lapsizm 21\n1 is 13\n3 my 14\n4 inst 3\n5 follow "
        "3\n6 me 3\n7 hello 3\n8 hello2 3\n9 hello3 3\n10 "
        "hello4 3\n11 hello5 9";

  PageContainer page;
  UsedMemory memory(page);
  StatSender stat(page);
  Histogram histogram(page);

  EXPECT_THROW(page.Load(ss, 0.1), std::runtime_error);

  EXPECT_EQ(page.GetRawData().size(), 0);
  EXPECT_EQ(page.GetData().size(), 0);
  EXPECT_EQ(memory.used(), 0);
  EXPECT_EQ(histogram.GetCountVec().empty(), true);
}

TEST(PageContainerTest, LoadStream_oops) {
  std::stringstream ss;

  ss << "1 @lapsizm 3\n2 is 13\n3 my 14\n4 inst 6\n5 follow "
        "8\n6 me 99\n7 hello 89\n8 hello2 78\n9 hello3 76\n10 "
        "hello 9";

  PageContainer page;
  UsedMemory memory(page);
  StatSender stat(page);
  Histogram histogram(page);

  EXPECT_THROW(page.Load(ss, 5), std::runtime_error);
  EXPECT_EQ(histogram.GetCountVec().empty(), true);
  EXPECT_EQ(page.GetRawData().size(), 0);
  EXPECT_EQ(page.GetData().size(), 0);
  EXPECT_EQ(memory.used(), 0);
}

TEST(PageContainer, CorrectInput) {
  PageContainer page;
  UsedMemory memory(page);
  StatSender stat(page);
  Histogram histogram(page);

  std::stringstream ss;

  ss << "1 @lapsizm 3\n2 is 10\n3 my 20\n4 inst 30\n5 follow "
        "40\n6 me 50\n7 hello 10\n8 hello2 20\n9 hello3 30\n10 "
        "hello 40\n11 HomePage 90";
  EXPECT_NO_THROW(page.Load(ss, 5));
  EXPECT_EQ(histogram.GetCountVec()[0], 1);
  EXPECT_EQ(histogram.GetAvgVec()[0], 34);
  EXPECT_EQ(page.GetRawData().size(), 11);
  EXPECT_EQ(page.GetData().size(), 10);
}

TEST(File, empty) {
  PageContainer page;
  UsedMemory memory(page);
  StatSender stat(page);
  Histogram histogram(page);

  std::stringstream ss;

  EXPECT_THROW(page.Load(ss, 0), std::runtime_error);
}

class MockStatSender : public StatSender {
 public:
  MockStatSender(PageContainer& p) : StatSender(p) {}
  MOCK_METHOD(void, AsyncSend, (const std::vector<Item>&, std::string_view),
              (override));
};

MATCHER(LoadedItem, "Our loaded items") {
  return arg == std::string_view("/items/loaded");
}
MATCHER(SkipItem, "Our Skipped Items") {
  return arg == std::string_view("/items/skipped");
}

TEST(PageContainer, IncorrectInput) {
  PageContainer page;
  UsedMemory memory(page);
  StatSender stat(page);
  Histogram histogram(page);

  MockStatSender mock_(page);

  std::stringstream ss;
  ss << "1 @lapsizm \n2  10\n3 my 20\n4 inst 30\n5 follow "
        "40\n6 me 50\n7 hello 10\n8 hello2 20\n9 hello3 30\n10 "
        "hello 40\n11 HomePage 90\n12 About 89";

  EXPECT_CALL(mock_, AsyncSend(testing::_, SkipItem())).Times(2);
  EXPECT_CALL(mock_, AsyncSend(testing::_, LoadedItem())).Times(1);
  page.Load(ss, 0.1);
}

TEST(StatSender, AsyncLoad) {
  PageContainer page;
  UsedMemory memory(page);
  StatSender stat(page);
  Histogram histogram(page);

  std::stringstream ss;
  ss << "1 @lapsizm 3\n2 is 10\n3 my 20\n4 inst 30\n5 follow "
        "40\n6 me 50\n7 hello 10\n8 hello2 20\n9 hello3 30\n10 "
        "hello 40\n11 HomePage 90";

  MockStatSender sender(page);

  EXPECT_CALL(sender, AsyncSend(testing::_, SkipItem())).Times(1);
  EXPECT_CALL(sender, AsyncSend(testing::_, LoadedItem())).Times(1);
  page.Load(ss, 4);

  std::stringstream ss2;
  ss2 << "1 @lapsizm 6\n2 is 10\n3 my 20\n4 inst 30\n5 follow "
         "40\n6 me 50\n7 hello 10\n8 hello2 20\n9 hello3 30\n10 "
         "hello 40\n11 HomePage 1\n12 About 2\n13 ello 3";
  EXPECT_CALL(sender, AsyncSend(testing::_, SkipItem())).Times(0);
  EXPECT_CALL(sender, AsyncSend(testing::_, LoadedItem())).Times(1);
  page.Load(ss2, 0);

  EXPECT_CALL(sender, AsyncSend(testing::_, SkipItem())).Times(3);
  EXPECT_CALL(sender, AsyncSend(testing::_, LoadedItem())).Times(1);
  page.Reload(4);
}

TEST(Memory, CorrectCount) {
  std::vector<Item> vec(12);
  for (size_t i = 0; i < 12; ++i) {
    vec[i].id = std::to_string(i);
    vec[i].name = "@lapsizm";
    vec[i].score = 3.2 + i;
  }

  size_t sum = 0;
  for (auto& item : vec) {
    sum += item.id.capacity() + item.name.capacity() + sizeof(item.score);
  }
  UsedMemory memory;
  std::vector<Item> vec_old = {};
  memory.OnDataLoaded(vec_old, vec);
  EXPECT_EQ(memory.used(), sum);

  std::vector<Item> vec1(5);
  for (size_t i = 0; i < 5; ++i) {
    vec[i].id = std::to_string(i + 2);
    vec[i].name = "@lap";
    vec[i].score = 14.2 + i;
  }

  size_t sum2 = 0;
  for (auto& item : vec1) {
    sum2 += item.id.capacity() + item.name.capacity() + sizeof(item.score);
  }

  memory.OnDataLoaded(vec, vec1);
  EXPECT_EQ(memory.used(), sum2);

  std::vector<std::string> vec2(15);
  for (size_t i = 0; i < 15; ++i) {
    vec2[i] = std::to_string(i + 2) + "@lap" + std::to_string(14.2 + i);
  }

  std::vector<std::string> vec_old_2 = {};
  for (auto& item : vec2) {
    sum2 += item.capacity();
  }
  memory.OnRawDataLoaded(vec_old_2, vec2);
  EXPECT_EQ(memory.used(), sum2);
}
