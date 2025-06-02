#include "ascii85_decoder.h"
#include <gtest/gtest.h>
#include <sstream>
#include <string>

TEST(ASCII85DecoderTest, BasicDecoding) {
    auto result = ASCII85Decoder::decode("9jqo^");
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], 'M');
    EXPECT_EQ(result[1], 'a');
    EXPECT_EQ(result[2], 'n');
    EXPECT_EQ(result[3], ' ');
}

TEST(ASCII85DecoderTest, SingleCharacter) {
    EXPECT_THROW(ASCII85Decoder::decode("A"), std::runtime_error);
}

TEST(ASCII85DecoderTest, MultipleZ) {
    auto result = ASCII85Decoder::decode("zzzz");
    ASSERT_EQ(result.size(), 16);
    for (int i = 0; i < 16; i += 4) {
        EXPECT_EQ(result[i], 0);
        EXPECT_EQ(result[i+3], 0);
    }
}


TEST(ASCII85DecoderTest, WhitespaceHandling) {
    auto result = ASCII85Decoder::decode(" 9 j q o ^ ");
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], 'M');
}

TEST(ASCII85DecoderTest, NewlineHandling) {
    auto result = ASCII85Decoder::decode("9jqo\n^");
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[2], 'n');
}

TEST(ASCII85DecoderTest, WithDelimiters) {
    std::istringstream input("<~9jqo^~>");
    std::ostringstream output;
    ASCII85Decoder::decodeStream(input, output);
    EXPECT_EQ(output.str(), "Man ");
}

TEST(ASCII85DecoderTest, MultipleBlocks) {
    auto result = ASCII85Decoder::decode("9jqo^BlbD");
    ASSERT_EQ(result.size(), 7);
    EXPECT_EQ(result[0], 'M');
    EXPECT_EQ(result[1], 'a');
    EXPECT_EQ(result[2], 'n');
    EXPECT_EQ(result[3], ' ');
    EXPECT_EQ(result[4], 'i');
    EXPECT_EQ(result[5], 's');
    EXPECT_EQ(result[6], ' ');
}

TEST(ASCII85DecoderTest, AllZeroesZ) {
    auto result = ASCII85Decoder::decode("z");
    ASSERT_EQ(result.size(), 4);
    for (auto b : result) EXPECT_EQ(b, 0);
}

TEST(ASCII85DecoderTest, MaxValue) {
    auto result = ASCII85Decoder::decode("zzzzz");
    ASSERT_EQ(result.size(), 20);
}


TEST(ASCII85DecoderTest, InvalidCharBelowRange) {
    EXPECT_NO_THROW(ASCII85Decoder::decode("9jqo "));
}

TEST(ASCII85DecoderTest, InvalidCharAboveRange) {
    EXPECT_THROW(ASCII85Decoder::decode("9jqo~"), std::runtime_error);
}

TEST(ASCII85DecoderTest, NonAsciiInput) {
    EXPECT_THROW(ASCII85Decoder::decode("9jqo\xFF"), std::runtime_error);
}


TEST(ASCII85DecoderTest, StreamSimple) {
    std::istringstream input("<~9jqo^~>");
    std::ostringstream output;
    ASCII85Decoder::decodeStream(input, output);
    EXPECT_EQ(output.str(), "Man ");
}

TEST(ASCII85DecoderTest, StreamChunked) {
    std::istringstream input("<~9jqo^BlbD~>");
    std::ostringstream output;
    ASCII85Decoder::decodeStream(input, output);
    EXPECT_EQ(output.str().substr(0, 4), "Man ");
}

TEST(ASCII85DecoderTest, StreamWithWhitespace) {
    std::istringstream input("<~9j qo^ ~>");
    std::ostringstream output;
    ASCII85Decoder::decodeStream(input, output);
    EXPECT_EQ(output.str().size(), 4);
}


TEST(ASCII85DecoderTest, EmptyInput) {
    auto result = ASCII85Decoder::decode("");
    EXPECT_TRUE(result.empty());
}

TEST(ASCII85DecoderTest, MinimalValidInput) {
    EXPECT_THROW(ASCII85Decoder::decode("A"), std::runtime_error);
}

TEST(ASCII85DecoderTest, LargeInput) {
    std::string large_input(1000, 'z');
    auto result = ASCII85Decoder::decode(large_input);
    ASSERT_EQ(result.size(), 4000);
}


TEST(ASCII85DecoderTest, Performance) {
    std::string test_data(100000, 'A');
    EXPECT_NO_THROW(ASCII85Decoder::decode(test_data));
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
TEST(ASCII85EncoderTest, BasicEncoding) {
    std::vector<uint8_t> data = {'M', 'a', 'n', ' '};
    std::string encoded = ASCII85Decoder::encode(data);
    EXPECT_EQ(encoded, "9jqo^");
}

TEST(ASCII85EncoderTest, ZeroBytes) {
    std::vector<uint8_t> data(4, 0);
    std::string encoded = ASCII85Decoder::encode(data);
    EXPECT_EQ(encoded, "z");
}

TEST(ASCII85EncoderTest, PartialGroup) {
    std::vector<uint8_t> data = {'A'};
    std::string encoded = ASCII85Decoder::encode(data);
    EXPECT_EQ(encoded.length(), 2);
}