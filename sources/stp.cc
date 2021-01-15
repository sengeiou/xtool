#include <QTimer>

#include "stp.h"

quint16 StpOpcode::CRC16(quint16 seed, const quint8 *src, size_t len)
{
    for (; len > 0; len--) {
        seed = (seed >> 8U) | (seed << 8U);
        seed ^= *src++;
        seed ^= (seed & 0xffU) >> 4U;
        seed ^= seed << 12U;
        seed ^= (seed & 0xffU) << 5U;
    }
    return seed;
}

void StpOpcode::Reset()
{
    nbuf_.Reset();
    nbuf_.Reserve(sizeof(StpHeader) + 1);
}

bool StpOpcode::ProcessMessage(const QByteArray &buf)
{
    const quint8 *data = reinterpret_cast<const quint8 *>(buf.constData());
    const StpHeader *hdr = reinterpret_cast<const StpHeader *>(data);
    quint16 len = Netbuffer::ToCpu16(hdr->length);
    quint16 stx = Netbuffer::ToCpu16(hdr->stx);
    quint16 etx;

    if (stx != StpHeader::STX)
        return false;

    len = Netbuffer::ToCpu16(hdr->length);
    if (len < STP_MIN_SIZE)
        return false;

    etx = Netbuffer::ByteToCpu16(&data[len - 2]);
    if (etx != StpHeader::ETX)
        return false;

    quint16 crc = Netbuffer::ByteToCpu16(&data[len - 4]);
    if (CRC16(0, &data[2], len - 6) != crc)
        return false;

    if (!hdr->upload)
        return false;

    if (!ConfirmRequest(hdr))
        return false;

    // Discard header and tail
    data += sizeof(StpHeader);
    len -= (sizeof(StpHeader) + 4);
    return true;
}

bool StpOpcode::ConfirmRequest(const StpHeader *header)
{
    const StpHeader *hdr = reinterpret_cast<const StpHeader *>(nbuf_.data());
    if (hdr->seqno == header->seqno &&
        hdr->version == header->version) {
        return true;
    }
    return false;
}

void StpOpcode::AppendMessage(int minor, const char *data, quint16 len)
{
    union {
       quint16 len;
       char buf[2];
    } m;

    if (len+3 > nbuf_.remain())
        return;

    char mr = static_cast<char>(minor);
    m.len = nbuf_.ToNet16(len);
    nbuf_.AddU8(mr);
    nbuf_.AddU8(m.buf[0]);
    nbuf_.AddU8(m.buf[1]);
    if (len > 0)
        nbuf_.AddMemory(data, len);
}

void StpOpcode::AppendHeader(int cls, quint16 flags)
{
    nbuf_.PushU8(static_cast<quint8>(cls));
    StpHeader *hdr = reinterpret_cast<StpHeader *>(nbuf_.Push(sizeof(StpHeader)));
    hdr->stx = nbuf_.ToNet16(StpHeader::STX);
    hdr->upload = 0;
    hdr->require_ack = (flags & NO_ACK)? 0: 1;
    hdr->request = 1;
    hdr->version = 0;
    hdr->seqno = nbuf_.ToNet16(reqno_);

    quint16 len = nbuf_.length() + 4;
    hdr->length = nbuf_.ToNet16(len);
    reqno_++;

    const quint8 *start = reinterpret_cast<const quint8 *>((nbuf_.data() + 2));
    quint16 crc = CRC16(0, start, nbuf_.length()-2);
    nbuf_.AddU16(nbuf_.ToNet16(crc));
    nbuf_.AddU16(nbuf_.ToNet16(StpHeader::ETX));
}

bool StpOpcode::GeneratePacket(int cls, quint16 flags, QByteArray *ba)
{
    AppendHeader(cls, flags);
    ba->append(nbuf_.data(), nbuf_.length());
    return true;
}
