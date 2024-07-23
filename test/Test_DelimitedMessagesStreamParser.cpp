#include <protobuf_parser/DelimitedMessagesStreamParser.h>
#include <protobuf_parser/helpers.h>
#include <protobuf/message.pb.h>
#include <gtest/gtest.h>

TEST(Parser, OneFastRequest)
{
    std::list<typename DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage>::PointerToConstValue> messages;
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;

    TestTask::Messages::WrapperMessage message;
    message.mutable_request_for_fast_response();

    auto data = serializeDelimited(message);
    messages = parser.parse(std::string(data->begin(), data->end()));
    ASSERT_EQ(1, messages.size());

    auto item = messages.front();
    ASSERT_TRUE(item->has_request_for_fast_response());
}

TEST(Parser, SomeFastRequests)
{
    std::list<typename DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage>::PointerToConstValue> messages;
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;

    TestTask::Messages::WrapperMessage message;
    message.mutable_request_for_fast_response();

    auto data = serializeDelimited(message);

    size_t count = 10;
    std::string stream;
    for (int i = 0; i < count; ++i)
        stream.append(data->begin(), data->end());

    messages = parser.parse(stream);
    ASSERT_EQ(count, messages.size());

    for (auto &item : messages)
        ASSERT_TRUE(item->has_request_for_fast_response());
}

TEST(Parser, OneSlowRequest)
{
    std::list<typename DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage>::PointerToConstValue> messages;
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;

    TestTask::Messages::WrapperMessage message;
    message.mutable_request_for_slow_response()->set_time_in_seconds_to_sleep(0);

    auto data = serializeDelimited(message);
    messages = parser.parse(std::string(data->begin(), data->end()));
    ASSERT_EQ(1, messages.size());

    auto item = messages.front();
    ASSERT_TRUE(item->has_request_for_slow_response());
    EXPECT_EQ(
            item->request_for_slow_response().time_in_seconds_to_sleep(),
            message.request_for_slow_response().time_in_seconds_to_sleep()
    );
}

TEST(Parser, SomeSlowRequests)
{
    std::list<typename DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage>::PointerToConstValue> messages;
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;

    TestTask::Messages::WrapperMessage message;
    message.mutable_request_for_slow_response()->set_time_in_seconds_to_sleep(0);

    auto data = serializeDelimited(message);

    size_t count = 10;
    std::string stream;
    for (int i = 0; i < count; ++i)
        stream.append(data->begin(), data->end());

    messages = parser.parse(stream);
    ASSERT_EQ(count, messages.size());

    for (auto &item : messages)
    {
        ASSERT_TRUE(item->has_request_for_slow_response());
        EXPECT_EQ(
                item->request_for_slow_response().time_in_seconds_to_sleep(),
                message.request_for_slow_response().time_in_seconds_to_sleep()
        );
    }
}

TEST(Parser, SomeRequests)
{
    std::list<typename DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage>::PointerToConstValue> messages;
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;

    TestTask::Messages::WrapperMessage fastRequest;
    fastRequest.mutable_request_for_fast_response();

    TestTask::Messages::WrapperMessage slowRequest;
    slowRequest.mutable_request_for_slow_response()->set_time_in_seconds_to_sleep(0);

    auto fReqData = serializeDelimited(fastRequest);
    auto sReqData = serializeDelimited(slowRequest);

    size_t count = 10;
    std::string stream;
    for (int i = 0; i < count; ++i)
        stream.append(std::rand() % 2 > 0 ? std::string(fReqData->begin(), fReqData->end()) : std::string(sReqData->begin(), sReqData->end()));

    messages = parser.parse(stream);
    ASSERT_EQ(count, messages.size());

    for (auto &item : messages)
    {
        ASSERT_TRUE(item->has_request_for_fast_response() || item->has_request_for_slow_response());
        if (item->has_request_for_slow_response())
        EXPECT_EQ(
                item->request_for_slow_response().time_in_seconds_to_sleep(),
                slowRequest.request_for_slow_response().time_in_seconds_to_sleep()
        );
    }
}
