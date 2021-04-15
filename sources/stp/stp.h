#ifndef XTOOL_STP_H_
#define XTOOL_STP_H_

#include <QByteArray>
#include <QList>
#include <cstring>

#define CPU_LITTLE_ENDIAN //??

#define STP_OTA_CLASS     0x01
#define STP_INFO_CLASS    0x02
#define STP_SETTING_CLASS 0x03
#define STP_GPS_CLASS     0x07
#define STP_REMIND_CLASS  0x08
#define STP_CALL_CLASS    0x09


#pragma pack(1)
struct OTAHeader {
    enum {
#ifdef CONFIG_PROTOBUF
        MAX_PAYLOAD = 256 - 7,
#else
        MAX_PAYLOAD = 256,
#endif
    };
    quint16 maxno;
    quint16 seqno;
};

#pragma pack(1)
struct StpL3Header {
    quint8 minor;
    quint16 length;
#define OPC_LEN(len) ((len) & OPC_LENMASK)
#define OPC_LENMASK  0x0FFF
#define OPC_SCODE    0x1000
#define OPC_SLEN(len) (OPC_LEN(len) | OPC_SCODE) /* Status code with lenght */
#define OPC_DLEN(len) ((quint16)(len)) /* */
    quint8 data[];
};

#pragma pack(1)
struct StpHeader {
    enum {
        STX = 0xDEEF,
        ETX = 0xBCCD
    };
    quint16 stx;
#if defined(CPU_LITTLE_ENDIAN)
    quint8 upload:1;
    quint8 request:1;
    quint8 require_ack:1;
    quint8 reserved:1;
    quint8 version:4;
#else
    quint8 version:4;
    quint8 reserved:1;
    quint8 require_ack:1;
    quint8 request:1;
    quint8 upload:1;
#endif
    quint16 length;
    quint16 seqno;
};

class Netbuffer {
public:
    enum {
      NET_BUFSIZE = 512
    };

    Netbuffer() {
        Reset();
    }
    ~Netbuffer() {}
    Netbuffer(const Netbuffer &) = delete;
    Netbuffer &operator=(const Netbuffer &) = delete;

    void Reset() {
        data_ = buffer_;
        len_ = 0;
    }
    void Reserve(quint16 size) {
        if (data_ + size > &buffer_[NET_BUFSIZE-1])
            return;
        data_ += size;
        len_ = 0;
    }
    char *Add(quint16 len) {
        char *ptr = data_ + len_;
        len_ += len;
        return ptr;
    }
    char *AddMemory(const char *p, quint16 len) {
        return (char *)std::memcpy(Add(len), p, len);
    }
    void AddU8(quint8 data) {
        quint8 *v = (quint8 *)Add(sizeof(data));
        *v = data;
    }
    void AddU16(quint16 data) {
        quint16 *v = (quint16 *)Add(sizeof(data));
        *v = data;
    }
    char *Push(quint16 len) {
        if ((data_ - len) < buffer_)
            return nullptr;
        data_ -= len;
        len_ += len;
        return data_;
    }
    void PushU8(quint8 data) {
        quint8 *v = (quint8 *)Push(sizeof(data));
        *v = data;
    }
    void PushU16(quint16 data) {
        quint16 *v = (quint16 *)Push(sizeof(data));
        *v = data;
    }
    char *Pull(quint16 len) {
        len_ -= len;
        data_ += len;
        return data_;
    }
    char *PullMemory(quint16 len) {
        char *ptr = data_;
        len_ -= len;
        data_ += len;
        return ptr;
    }
    static quint16 ToNet16(quint16 data) {
     #ifdef CPU_LITTLE_ENDIAN
        return (data << 8) | (data >> 8);
     #else
        return data;
     #endif
    }
    static quint32 ToNet32(quint32 data) {
     #ifdef CPU_LITTLE_ENDIAN
        quint32 l = ToNet16(data & 0xFFFF);
        quint16 h = ToNet16((data >> 16) & 0xFFFF);
        return (l << 16) | h;
     #else
        return data;
     #endif
    }
    static quint16 ToCpu16(quint16 data) {
     #ifdef CPU_LITTLE_ENDIAN
        return (data << 8) | (data >> 8);
     #else
        return data;
     #endif
    }
    static quint32 ToCpu32(quint32 data) {
     #ifdef CPU_LITTLE_ENDIAN
        quint32 l = ToCpu16(data & 0xFFFF);
        quint16 h = ToCpu16((data >> 16) & 0xFFFF);
        return (l << 16) | h;
     #else
        return data;
     #endif
    }
    static quint16 ByteToCpu16(const quint8 *bytes) {
     #ifdef CPU_LITTLE_ENDIAN
        return ((quint16)bytes[0] << 8) | bytes[1];
     #else
        return ((quint16)bytes[1] << 8) | bytes[0];
     #endif
    }

    char *data() const {
        return data_;
    }
    quint16 length() const {
        return len_;
    }
    quint16 remain() const {
        return NET_BUFSIZE - len_;
    }

private:
    char buffer_[NET_BUFSIZE];
    char *data_;
    quint16 len_;
};


class StpOpcode {
public:
    enum {
        OPCODE_OTA
    };
    enum {
        NO_ACK = 1
    };

    StpOpcode() : nbuf_(), reqno_(0) {
        //Reset();
    }
    ~StpOpcode() {}

    void Reset();
    void AppendMessage(int minor, const char *data, quint16 len);
    bool GeneratePacket(int cls, quint16 flags, QByteArray *ba);
    bool ProcessMessage(const QByteArray &);
    quint16 CRC16(quint16 seed, const quint8 *src, size_t len);
    const quint8 *ToL2(const QByteArray &buf, quint16 *len);

private:
    void AppendHeader(int cls, quint16 flags);
    bool ConfirmRequest(const StpHeader *header);

private:
    Netbuffer nbuf_;
    quint16 reqno_;
    quint16 rxlen_;
};



#endif //XTOOL_STP_H_
