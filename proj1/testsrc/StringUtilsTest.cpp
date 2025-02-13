#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    EXPECT_EQ(Slice("hello", 1, 4), "ell");
    EXPECT_EQ(Slice("hello", -3, 0), "llo");
    EXPECT_EQ(Slice("hello", 0, 0), "hello");
    EXPECT_EQ(Slice("hello", 2, -1), "ll");
}

TEST(StringUtilsTest, Capitalize){
    EXPECT_EQ(Capitalize("hello"), "Hello");
    EXPECT_EQ(Capitalize("Hello"), "Hello");
    EXPECT_EQ(Capitalize("hELLO"), "Hello");
}

TEST(StringUtilsTest, Upper){
    EXPECT_EQ(Upper("hello"), "HELLO");
    EXPECT_EQ(Upper("Hello"), "HELLO");
    EXPECT_EQ(Upper("HELLO"), "HELLO");
}

TEST(StringUtilsTest, Lower){
    EXPECT_EQ(Lower("hello"), "hello");
    EXPECT_EQ(Lower("Hello"), "hello");
    EXPECT_EQ(Lower("HELLO"), "hello");
}

TEST(StringUtilsTest, LStrip){
    EXPECT_EQ(LStrip("  hello"), "hello");
    EXPECT_EQ(LStrip("\t\nhello"), "hello");
    EXPECT_EQ(LStrip("hello"), "hello");
}

TEST(StringUtilsTest, RStrip){
    EXPECT_EQ(RStrip("hello  "), "hello");
    EXPECT_EQ(RStrip("hello\t\n"), "hello");
    EXPECT_EQ(RStrip("hello"), "hello");
}

TEST(StringUtilsTest, Strip){
    EXPECT_EQ(Strip("  hello  "), "hello");
    EXPECT_EQ(Strip("\t\nhello\t\n"), "hello");
    EXPECT_EQ(Strip("hello"), "hello");
}

TEST(StringUtilsTest, Center){
    EXPECT_EQ(Center("hello", 10, '-'), "---hello---");
    EXPECT_EQ(Center("hello", 9, '-'), "---hello--");
    EXPECT_EQ(Center("hello", 5, '-'), "hello");
}

TEST(StringUtilsTest, LJust){
    EXPECT_EQ(LJust("hello", 10, '-'), "hello-----");
    EXPECT_EQ(LJust("hello", 5, '-'), "hello");
}

TEST(StringUtilsTest, RJust){
    EXPECT_EQ(RJust("hello", 10, '-'), "-----hello");
    EXPECT_EQ(RJust("hello", 5, '-'), "hello");
}

TEST(StringUtilsTest, Replace){
    EXPECT_EQ(Replace("hello sahib", "sahib", "mike"), "hello mike");
    EXPECT_EQ(Replace("hello sahib there", "sahib", "there"), "hello there there");
    EXPECT_EQ(Replace("hello", "sahib", "there"), "hello");
}

TEST(StringUtilsTest, Split){
    EXPECT_EQ(Split("hello sahib"), std::vector<std::string>({"hello", "sahib"}));
    EXPECT_EQ(Split("hello  sahib", " "), std::vector<std::string>({"hello", "", "sahib"}));
    EXPECT_EQ(Split("hello"), std::vector<std::string>({"hello"}));
}

TEST(StringUtilsTest, Join){
    EXPECT_EQ(Join("-", {"hello", "sahib"}), "hello-sahib");
    EXPECT_EQ(Join("", {"hello", "sahib"}), "hellosahib");
    EXPECT_EQ(Join("-", {}), "");
}

TEST(StringUtilsTest, ExpandTabs){
    EXPECT_EQ(ExpandTabs("hello\tsahib", 4), "hello   sahib");
    EXPECT_EQ(ExpandTabs("hello\tsahib", 8), "hello       sahib"); 
}

TEST(StringUtilsTest, EditDistance){
    EXPECT_EQ(EditDistance("kitten", "sitting"), 3);
    EXPECT_EQ(EditDistance("kitten", "kitten"), 0);
    EXPECT_EQ(EditDistance("kitten", "sitten", true), 1);
}
