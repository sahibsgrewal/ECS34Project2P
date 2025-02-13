#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    
    EXPECT_EQ(StringUtils::Slice("",1,2), "");
    EXPECT_EQ(StringUtils::Slice("hello", 1, 4), "ell");
    EXPECT_EQ(StringUtils::Slice("hello", -4, -1), "ell");
    EXPECT_EQ(StringUtils::Slice("hello", 1), "ello");
    EXPECT_EQ(StringUtils::Slice("hello", -5, 0), "hello");

}

TEST(StringUtilsTest, Capitalize){
    EXPECT_EQ(StringUtils::Capitalize("hello"), "Hello");
    EXPECT_EQ(StringUtils::Capitalize("all around the world"), "All around the world");
    EXPECT_EQ(StringUtils::Capitalize("278"), "278");
    EXPECT_EQ(StringUtils::Capitalize("CHECK"), "Check");

}

TEST(StringUtilsTest, Upper){

    EXPECT_EQ(StringUtils::Upper("ALL AROUND THE WORLD"), "ALL AROUND THE WORLD");
    EXPECT_EQ(StringUtils::Upper("278"), "278");
    EXPECT_EQ(StringUtils::Upper("CHECK"), "CHECK");
    
    
}

TEST(StringUtilsTest, Lower){
    
    EXPECT_EQ(StringUtils::Lower("hello"), "hello");
    EXPECT_EQ(StringUtils::Lower("ALL AROUND THE WORLD"), "all around the world");
    EXPECT_EQ(StringUtils::Lower("278"), "278");
    EXPECT_EQ(StringUtils::Lower("CHECK"), "check");
    
}

TEST(StringUtilsTest, LStrip){

    EXPECT_EQ(StringUtils::LStrip("  hello  "), "hello  ");

}

TEST(StringUtilsTest, RStrip){
    EXPECT_EQ(StringUtils::RStrip("  hello  "), "  hello");
}

TEST(StringUtilsTest, Strip){
   EXPECT_EQ(StringUtils::Strip("  hello  "), "hello");
}

TEST(StringUtilsTest, Center){
    
    EXPECT_EQ(StringUtils::Center("hello", 11), "   hello   ");
    EXPECT_EQ(StringUtils::Center("hello", 10, '*'), "**hello***");
    EXPECT_EQ(StringUtils::Center("hello", 5), "hello");  // No padding
    EXPECT_EQ(StringUtils::Center("hello", 3), "hello");  // Width less than string size
    EXPECT_EQ(StringUtils::Center("", 6, '-'), "------"); // Center empty string
    

}

TEST(StringUtilsTest, LJust){
    EXPECT_EQ(StringUtils::LJust("hello", 10), "hello     ");
    EXPECT_EQ(StringUtils::LJust("hello", 10, '*'), "hello*****");
    EXPECT_EQ(StringUtils::LJust("hello", 5), "hello");  // No padding
    EXPECT_EQ(StringUtils::LJust("hello", 3), "hello");  // Width less than string size
    EXPECT_EQ(StringUtils::LJust("", 6, '-'), "------"); // LJust empty string

}

TEST(StringUtilsTest, RJust){
    EXPECT_EQ(StringUtils::RJust("hello", 10), "     hello");
    EXPECT_EQ(StringUtils::RJust("hello", 10, '*'), "*****hello");
    EXPECT_EQ(StringUtils::RJust("hello", 5), "hello");  // No padding
    EXPECT_EQ(StringUtils::RJust("hello", 3), "hello");  // Width less than string size
    EXPECT_EQ(StringUtils::RJust("", 6, '-'), "------"); // RJust empty string
}

TEST(StringUtilsTest, Replace){
   EXPECT_EQ(StringUtils:: Replace("hello world", "world", "C++"), "hello C++");
}

TEST(StringUtilsTest, Split){
    
EXPECT_EQ(StringUtils::Split("a,b,c", ","), (std::vector<std::string>{"a", "b", "c"}));
EXPECT_EQ(StringUtils::Split("a,,b,c", ","), (std::vector<std::string>{"a", "", "b", "c"}));
EXPECT_EQ(StringUtils::Split("a:b:c", ":"), (std::vector<std::string>{"a", "b", "c"}));
EXPECT_EQ(StringUtils::Split("a b c", " "), (std::vector<std::string>{"a", "b", "c"}));

    
    EXPECT_EQ(StringUtils::Split("", ","), std::vector<std::string>{""}); // Empty string
}

TEST(StringUtilsTest, Join){
    EXPECT_EQ(StringUtils::Join(",", {"a", "b", "c"}), "a,b,c");
    EXPECT_EQ(StringUtils::Join(":", {"a", "b", "c"}), "a:b:c");
    EXPECT_EQ(StringUtils::Join("-", {"a"}), "a");
    EXPECT_EQ(StringUtils::Join(",", {}), "");  // Empty vector
    EXPECT_EQ(StringUtils::Join("", {"a", "b", "c"}), "abc");  // No delimiter
    
}

TEST(StringUtilsTest, ExpandTabs){
    EXPECT_EQ(StringUtils::ExpandTabs("a\tb\tc", 1), "a b c");  // Minimal tab size
    EXPECT_EQ(StringUtils::ExpandTabs("", 4), "");              // Empty string
    EXPECT_EQ(StringUtils::ExpandTabs("abc", 4), "abc");        // No tabs
    
    
}

TEST(StringUtilsTest, EditDistance){
    EXPECT_EQ(StringUtils::EditDistance("kitten", "sitting", false), 3);
    EXPECT_EQ(StringUtils::EditDistance("flaw", "lawn", false), 2);
    EXPECT_EQ(StringUtils::EditDistance("hello", "HELLO", true), 0);  // Case-insensitive
    EXPECT_EQ(StringUtils::EditDistance("", "abc", false), 3);        // Empty left string
    EXPECT_EQ(StringUtils::EditDistance("abc", "", false), 3);        // Empty right string
    EXPECT_EQ(StringUtils::EditDistance("", "", false), 0);           // Both strings empty
    
}
