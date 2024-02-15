#include "telegrambuffer.h"

namespace comm {

void TelegramBuffer::append(const ByteArray& bytes)
{
    m_rawBuffer.append(bytes);
}

bool TelegramBuffer::checkRawBuffer()
{
    std::size_t signatureStartIndex = m_rawBuffer.findSequence(TelegramHeader::SIGNATURE, TelegramHeader::SIGNATURE_SIZE);
    if (signatureStartIndex != std::size_t(-1)) {
        std::size_t index = signatureStartIndex;
        if (index != 0) {
            m_rawBuffer.erase(m_rawBuffer.begin(), m_rawBuffer.begin()+index);
        }
        return true;
    }
    return false;
}

void TelegramBuffer::takeTelegramFrames(std::vector<comm::TelegramFramePtr>& result)
{
    if (m_rawBuffer.size() < TelegramHeader::size()) {
        return;
    }
    if (!m_headerOpt) {
        if (checkRawBuffer()) {
            TelegramHeader header(m_rawBuffer);
            if (header.isValid()) {
                m_headerOpt = std::move(header);
            }
        }
    }

    if (m_headerOpt) {
        const TelegramHeader& header = m_headerOpt.value();
        if (m_rawBuffer.size() >= TelegramHeader::size() + header.bodyBytesNum()) {
            ByteArray data(m_rawBuffer.begin() + TelegramHeader::size(), m_rawBuffer.begin() + TelegramHeader::size() + header.bodyBytesNum());
            uint32_t actualCheckSum = data.calcCheckSum();
            if (actualCheckSum == header.bodyCheckSum()) {
                TelegramFramePtr telegramFramePtr = std::make_shared<TelegramFrame>(TelegramFrame{header, std::move(data)});
                data.clear();
                result.push_back(telegramFramePtr);
            } else {
                m_errors.push_back("wrong checkSums " + std::to_string(actualCheckSum) +" for " + header.info() + " , drop this chunk");
            }
            m_rawBuffer.erase(m_rawBuffer.begin(), m_rawBuffer.begin()+TelegramHeader::size()+header.bodyBytesNum());
            m_headerOpt.reset();
        }
    }
}

std::vector<comm::TelegramFramePtr> TelegramBuffer::takeTelegramFrames()
{
    std::vector<comm::TelegramFramePtr> result;
    takeTelegramFrames(result);
    return result;
}

void TelegramBuffer::takeErrors(std::vector<std::string>& errors)
{
    errors.clear();
    std::swap(errors, m_errors);
}

} // namespace comm
