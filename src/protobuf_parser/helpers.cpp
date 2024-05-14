#include "helpers.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

template <typename Message>
PointerToConstData serializeDelimited(const Message& msg) {
    const size_t messageSize = msg.ByteSizeLong();
    const size_t headerSize = google::protobuf::io::CodedOutputStream::VarintSize32(messageSize);

    PointerToData result = std::make_shared<Data>(headerSize + messageSize);
    google::protobuf::io::ArrayOutputStream rawOutput(reinterpret_cast<char*>(&(*result)[0]), result->size());
    google::protobuf::io::CodedOutputStream codedOutput(&rawOutput);
    codedOutput.WriteVarint32(messageSize);
    msg.SerializeToCodedStream(&codedOutput);

    return result;
}

template <>
PointerToConstData serializeDelimited<WrapperMessage>(const WrapperMessage& msg) {
    // You can provide a specialized implementation for WrapperMessage if needed
    // Or use the default implementation
    return serializeDelimited<WrapperMessage>(msg);
}

// Explicit instantiation of serializeDelimited for required message types
template PointerToConstData serializeDelimited<FastResponse>(const FastResponse& msg);
template PointerToConstData serializeDelimited<SlowResponse>(const SlowResponse& msg);
template PointerToConstData serializeDelimited<RequestForFastResponse>(const RequestForFastResponse& msg);
template PointerToConstData serializeDelimited<RequestForSlowResponse>(const RequestForSlowResponse& msg);
