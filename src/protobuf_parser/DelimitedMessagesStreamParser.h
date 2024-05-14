#ifndef SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_H_
#define SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_H_

#include <list>
#include <string>
#include <memory>
#include "helpers.h"
#include "message.pb.h" // Include your generated protobuf message header file

template <typename MessageType>
class DelimitedMessagesStreamParser
{
public:
    typedef std::shared_ptr<const MessageType> PointerToConstValue;

    std::list<PointerToConstValue> parse(const std::string& data);

private:
    std::vector<char> m_buffer;
};

#endif //SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_H_
