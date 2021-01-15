#include <mutex>
#include <condition_variable>

#include "dlistbuf.h"


ByteArrayNodePool::ByteArrayNodePool(int num, size_t size)
    : list_(),
      mtx_(new std::mutex()),
      cv_(new std::condition_variable)
{
    while (num > 0) {
        ByteArrayNode *n = new ByteArrayNode(size, this);
        list_.Append(n);
        num--;
    }
}

ByteArrayNodePool::~ByteArrayNodePool()
{
    Destory();
    delete mtx_;
    delete cv_;
}

void ByteArrayNodePool::Destory()
{
    std::unique_lock<std::mutex> lock(*mtx_);
    list_.IterateSafe([](ByteArrayNode *node) {
        node->Remove();
        delete node;
    });
}

ByteArrayNode *ByteArrayNodePool::Allocate(bool wait)
{
    std::unique_lock<std::mutex> lock(*mtx_);
    ByteArrayNode *node;
    if (empty()) {
        if (!wait)
            return nullptr;
        do {
            cv_->wait(lock);
        } while (empty());
    }
    auto *n = list_.head();
    list_.Remove(n);
    mtx_->unlock();
    node = n->value();
    node->data()->clear();
    return node;
}

void ByteArrayNodePool::Release(ByteArrayNode *node)
{
    std::unique_lock<std::mutex> lock(*mtx_);
    if (empty()) {
        list_.Prepend(node);
        cv_->notify_one();
    } else {
        list_.Prepend(node);
    }
}
