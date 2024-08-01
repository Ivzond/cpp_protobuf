#include "protobuf_parser/helpers.hpp"
#include <message.pb.h>

#include <gtest/gtest.h>

TEST(ParseDelimited, DefaultTest)
{
    std::shared_ptr<TestTask::Messages::WrapperMessage> delimited;

    TestTask::Messages::WrapperMessage message;
    message.mutable_request_for_fast_response();

    auto buffer = serializeDelimited(message);
    size_t bytesConsumed = 0;

    delimited = parseDelimited<TestTask::Messages::WrapperMessage>(
            buffer->data(),
            buffer->size(),
            &bytesConsumed
    );

    ASSERT_FALSE(delimited == nullptr);
    EXPECT_TRUE(delimited->has_request_for_fast_response());
    EXPECT_EQ(bytesConsumed, buffer->size());
}

TEST(ParseDelimited, NullDataTest)
{
    std::shared_ptr<TestTask::Messages::WrapperMessage> delimited;

    size_t bytesConsumed = 0;

    delimited = parseDelimited<TestTask::Messages::WrapperMessage>(
            nullptr,
            0,
            &bytesConsumed
    );

    ASSERT_TRUE(delimited == nullptr);
    EXPECT_EQ(bytesConsumed, 0);
}

TEST(ParseDelimited, EmptyDataTest)
{
    std::shared_ptr<TestTask::Messages::WrapperMessage> delimited;

    size_t bytesConsumed = 0;

    delimited = parseDelimited<TestTask::Messages::WrapperMessage>(
            "",
            0,
            &bytesConsumed
    );

    ASSERT_TRUE(delimited == nullptr);
    EXPECT_EQ(bytesConsumed, 0);
}

TEST(ParseDelimited, WrongDataTest)
{
    std::string buffer = "\x05wrong";
    size_t bytesConsumed = 0;

    EXPECT_THROW(
            parseDelimited<TestTask::Messages::WrapperMessage>(buffer.data(), buffer.size(), &bytesConsumed),
            std::runtime_error
    );
}

TEST(ParseDelimited, CorruptedDataTest)
{
    TestTask::Messages::WrapperMessage message;
    message.mutable_request_for_fast_response();

    auto buffer = serializeDelimited(message);
    std::string corrupted = std::string(buffer->begin(), buffer->end());
    corrupted[0] -= 1;

    EXPECT_THROW(
            parseDelimited<TestTask::Messages::WrapperMessage>(corrupted.data(), corrupted.size()),
            std::runtime_error
    );
}

TEST(ParseDelimited, WrongMessageSizeTest)
{
    TestTask::Messages::WrapperMessage message;
    message.mutable_request_for_fast_response();

    auto buffer = serializeDelimited(message);
    size_t bytesConsumed = 0;

    auto delimited = parseDelimited<TestTask::Messages::WrapperMessage>(
            buffer->data(),
            buffer->size() / 2,
            &bytesConsumed
    );
    ASSERT_TRUE(delimited == nullptr);
    EXPECT_EQ(bytesConsumed, 0);
}
