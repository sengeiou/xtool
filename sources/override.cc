#include <cstdint>
#include <cstdlib>
#include <cstddef>

#include <mutex>

#include "dlist.h"

class ResourceNode : public ListNode<ResourceNode> {
public:
    typedef void (*release_t)(void *);
    typedef void (* const const_release_t)(void *);
    ResourceNode(void (*free)(void *), std::size_t size) {
        dealloc_ = free;
        size_ = size;
    }
    ~ResourceNode() {
        Free();
    }
    void Free() {
        dealloc_(this);
    }
    std::size_t size() const {
        return size_;
    }
    void *data() {
        return static_cast<void *>(data_);
    }
    release_t free() {
        return dealloc_;
    }

private:
    release_t dealloc_;
    std::size_t size_;
public:
    std::uint8_t data_[0];
};

class ResourceManager {
public:
    ResourceManager() :
        list_(),
        mtx_(),
        allocated_size_(0),
        allocated_count_(0) {
    }
    ~ResourceManager() {
        Clear();
    }
    void AddNode(ResourceNode *node) {
        mtx_.lock();
        allocated_size_ += node->size();
        allocated_count_++;
        list_.Prepend(node);
        mtx_.unlock();
    }
    void RemoveNode(ResourceNode *node) {
        mtx_.lock();
        allocated_size_ -= node->size();
        allocated_count_--;
        list_.Remove(node);
        mtx_.unlock();
    }
    void FreeNode(ResourceNode *node) {
        RemoveNode(node);
        node->Free();
    }
    void Clear() {
        list_.IterateSafe([this](ResourceNode *node) {
            FreeNode(node);
        });
    }

private:
    DList<ResourceNode> list_;
    std::mutex mtx_;
    std::size_t allocated_size_;
    std::size_t allocated_count_;
};

ResourceManager res_manager;

void *operator new(std::size_t size)
{
    std::size_t len = size + sizeof(ResourceNode);
    void *ptr = std::malloc(len);
    if (ptr) {
        ResourceNode *res = new (ptr) ResourceNode(&std::free, len);
        res_manager.AddNode(res);
        return res->data();
    } else {
        static const std::bad_alloc nomem;
        throw nomem;
    }
}

void operator delete(void *ptr)
{
    if (ptr) {
        ResourceNode *res = container_of(ptr,
            ResourceNode, data_);
        if (res->free() == &std::free)
            res_manager.FreeNode(res);
    }
}

void *operator new[](std::size_t size)
{
    return ::operator new(size);
}

void operator delete[](void *ptr)
{
    return ::operator delete(ptr);
}
