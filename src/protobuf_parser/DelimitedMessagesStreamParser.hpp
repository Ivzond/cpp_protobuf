#ifndef SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_H_
#define SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_H_

#include <list>
#include <string>
#include <memory>
#include <vector>
#include "helpers.hpp"

template <typename MessageType>
class DelimitedMessagesStreamParser {
public:
    typedef std::shared_ptr<const MessageType> PointerToConstValue;

    std::list<PointerToConstValue> parse(const std::string& data) {
        m_buffer.insert(m_buffer.end(), data.begin(), data.end());

        std::list<PointerToConstValue> msgs;
        size_t bytesRead = 0;

        while (true) {
            auto message = parseDelimited<MessageType>(m_buffer.data(), m_buffer.size(), &bytesRead);
            if (!message) break;

            msgs.push_back(message);
            m_buffer.erase(m_buffer.begin(), m_buffer.begin() + bytesRead);
        }

        return msgs;
    }

private:
    std::vector<char> m_buffer;
};

#endif /* SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_H_ */
