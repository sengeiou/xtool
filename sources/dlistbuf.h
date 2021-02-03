#ifndef DLISTBUF_H_
#define DLISTBUF_H_

#include <QByteArray>

#include "dlist.h"

namespace std {
class mutex;
class condition_variable;
}

class ByteArrayNode;

class ByteArrayNodePool {
public:
    explicit ByteArrayNodePool(int num, size_t size = 512);
    ByteArrayNodePool() = delete;
    ~ByteArrayNodePool();
    ByteArrayNodePool(const ByteArrayNodePool &) = delete;
    ByteArrayNodePool& operator=(const ByteArrayNodePool &) = delete;

    ByteArrayNode *Allocate(bool wait = true);
    void Release(ByteArrayNode *node);

private:
    void Destory();
    bool empty() {
        return list_.empty();
    }

private:
    DList<ByteArrayNode> list_;
    std::mutex *mtx_;
    std::condition_variable *cv_;
};

class ByteArrayNode : public ListNode<ByteArrayNode> {
public:
    ByteArrayNode(int size, ByteArrayNodePool *parent)
        : ListNode(), buf_(new QByteArray), parent_(parent) {
        buf_->resize(size);
    }
    ~ByteArrayNode() {
        delete buf_;
    }
    QByteArray *data() {
        return buf_;
    }
    void Release() {
        parent_->Release(this);
    }

private:
    QByteArray *buf_;
    ByteArrayNodePool *parent_;
};

class ByteArrayList {
public:
    ByteArrayList() : list_() {

    }
    ~ByteArrayList() {

    }
    void Append(ByteArrayNode *node) {
        list_.Append(node);
    }
    ByteArrayNode *TakeFirst() {
        auto *n = list_.head();
        list_.Remove(n);
        return n->value();
    }
    bool empty() const {
        return list_.empty();
    }

private:
    DList<ByteArrayNode> list_;
};


#endif //DLISTBUF_H_
