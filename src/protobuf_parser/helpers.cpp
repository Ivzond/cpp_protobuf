#include "helpers.hpp"
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

template <typename Message>
PointerToConstData serializeDelimited(const Message& msg) {
    size_t size = PROTOBUF_MESSAGE_BYTE_SIZE(msg);
    size_t varint_size = google::protobuf::io::CodedOutputStream::VarintSize32(size);
    PointerToConstData buffer = std::make_shared<Data>(varint_size + size);
    google::protobuf::io::ArrayOutputStream array_output(buffer->data(), buffer->size());
    google::protobuf::io::CodedOutputStream coded_output(&array_output);
    coded_output.WriteVarint32(size);
    msg.SerializeToCodedStream(&coded_output);
    return buffer;
}

template <typename Message>
std::shared_ptr<Message> parseDelimited(const void* data, size_t size, size_t* bytesConsumed) {
    google::protobuf::io::ArrayInputStream array_input(data, size);
    google::protobuf::io::CodedInputStream coded_input(&array_input);
    uint32_t message_size;
    if (!coded_input.ReadVarint32(&message_size)) {
        return nullptr;
    }
    if (coded_input.CurrentPosition() + message_size > size) {
        return nullptr;
    }
    std::shared_ptr<Message> message = std::make_shared<Message>();
    if (!message->ParseFromCodedStream(&coded_input)) {
        return nullptr;
    }
    if (bytesConsumed) {
        *bytesConsumed = coded_input.CurrentPosition();
    }
    return message;
}

// Эксплицитные инстанциации шаблонных функций
template PointerToConstData serializeDelimited<TestTask::Messages::FastResponse>(const TestTask::Messages::FastResponse&);
template std::shared_ptr<TestTask::Messages::FastResponse> parseDelimited<TestTask::Messages::FastResponse>(const void*, size_t, size_t*);
template PointerToConstData serializeDelimited<TestTask::Messages::WrapperMessage>(const TestTask::Messages::WrapperMessage&);
template std::shared_ptr<TestTask::Messages::WrapperMessage> parseDelimited<TestTask::Messages::WrapperMessage>(const void*, size_t, size_t*);
