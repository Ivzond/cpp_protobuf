#include "DelimitedMessagesStreamParser.h"
#include "helpers.h"
#include "../protobuf/message.pb.h"

template <typename MessageType>
std::list<typename DelimitedMessagesStreamParser<MessageType>::PointerToConstValue>
DelimitedMessagesStreamParser<MessageType>::parse(const std::string& data) {
    std::list<PointerToConstValue> messages;
    m_buffer.insert(m_buffer.end(), data.begin(), data.end());
    size_t offset = 0;
    while (offset < m_buffer.size()) {
        size_t bytes_consumed = 0;
        auto message = parseDelimited<MessageType>(m_buffer.data() + offset, m_buffer.size() - offset, &bytes_consumed);
        if (!message) {
            // Если нет сообщения, прерываем цикл и ожидаем больше данных
            break;
        }
        messages.push_back(message);
        offset += bytes_consumed;
    }
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + offset);
    return messages;
}

// Эксплицитные инстанциации шаблонных функций
template class DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage>;
