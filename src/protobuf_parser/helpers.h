#ifndef SRC_PROTOBUF_PARSER_HELPERS_H_
#define SRC_PROTOBUF_PARSER_HELPERS_H_

#include <vector>
#include <memory>
#include <google/protobuf/io/coded_stream.h>
#include "protobuf/message.pb.h"

#if GOOGLE_PROTOBUF_VERSION >= 3012004
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSizeLong())
#else
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSize())
#endif
typedef std::vector<char> Data;
typedef std::shared_ptr<const Data> PointerToConstData;
typedef std::shared_ptr<Data> PointerToData;

template <typename Message>
PointerToConstData serializeDelimited(const Message& msg){

    const size_t messageSize = PROTOBUF_MESSAGE_BYTE_SIZE(msg);
    const size_t varint_size = google::protobuf::io::CodedOutputStream::VarintSize32(messageSize);

    const PointerToData& result = std::make_shared<Data>(messageSize + varint_size);
    auto* buffer = reinterpret_cast<google::protobuf::uint8*>(&*result->data());

    google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(messageSize, buffer);
    msg.SerializeWithCachedSizesToArray(buffer + varint_size);
    return result;
}

/*!
 * \brief Расшифровывает сообщение, предваренное длиной из массива байтов.
 *
 * \tparam Message Тип сообщения, для работы с которым предназначена данная
 * функция.
 *
 * \param data Указатель на буфер данных.
 * \param size Размер буфера данных.
 * \param bytesConsumed Количество байт, которое потребовалось для расшифровки
 * сообщения в случае успеха.
 *
 * \return Умный указатель на сообщение. Если удалось расшифровать сообщение, то
 * он не пустой.
 */
template <typename Message>
std::shared_ptr<Message> parseDelimited(const void* data, size_t size, size_t* bytesConsumed = nullptr){
    if(!data || size == 0) {
        if (bytesConsumed) *bytesConsumed = 0;
        return nullptr;
    }

    google::protobuf::io::CodedInputStream input(reinterpret_cast<const google::protobuf::uint8*>(data), size);
    google::protobuf::uint32 messageSize;
    if (!input.ReadVarint32(&messageSize)) {
        throw std::runtime_error("Failed to read message size");
    }

    size_t totalSize = input.CurrentPosition() + messageSize;
    if (size < totalSize) {
        if (bytesConsumed) *bytesConsumed = 0;
        return nullptr;
    }

    auto message =std::make_shared<Message>();
    google::protobuf::io::CodedInputStream::Limit limit = input.PushLimit(messageSize);
    if (!message->ParseFromCodedStream(&input) || !input.ConsumedEntireMessage()) {
        throw std::runtime_error("Failed to parse message");
    }
    input.PopLimit(limit);

    if (bytesConsumed) *bytesConsumed = totalSize;
    return message;
}
#endif /* SRC_PROTOBUF_PARSER_HELPERS_H_ */