#include "DelimitedMessagesStreamParser.h"
#include <google/protobuf/io/coded_stream.h>

template <typename MessageType>
std::list<typename DelimitedMessagesStreamParser<MessageType>::PointerToConstValue>
DelimitedMessagesStreamParser<MessageType>::parse(const std::string& data) {
    std::list<PointerToConstValue> result;

    size_t bytesConsumed = 0;
    m_buffer.insert(m_buffer.end(), data.begin(), data.end());

    while (!m_buffer.empty()) {
        google::protobuf::io::CodedInputStream input(reinterpret_cast<const google::protobuf::uint8*>(m_buffer.data()), m_buffer.size());
        google::protobuf::uint32 messageSize;
        if (!input.ReadVarint32(&messageSize)) {
            break; // Incomplete message, exit the loop
        }

        if (input.ExpectAtEnd()) {
            break; // Incomplete message, exit the loop
        }

        if (input.BytesUntilLimit() < messageSize) {
            break; // Incomplete message, exit the loop
        }

        // Successfully read message size
        bytesConsumed += google::protobuf::io::CodedOutputStream::VarintSize32(messageSize);
        auto limit = input.PushLimit(messageSize);

        std::shared_ptr<MessageType> message = std::make_shared<MessageType>();
        if (message->ParseFromCodedStream(&input)) {
            result.push_back(message);
            bytesConsumed += messageSize;
        }

        input.PopLimit(limit);
    }

    // Erase consumed bytes from buffer
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + bytesConsumed);

    return result;
}

// Explicit instantiation of DelimitedMessagesStreamParser for required message types
template class DelimitedMessagesStreamParser<WrapperMessage>;
