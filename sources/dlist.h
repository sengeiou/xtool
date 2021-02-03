#ifndef DLIST_H_
#define DLIST_H_

#include <cstddef>
#include <functional>

#define cc_offsetof(type, member) \
    ((size_t)((char*)(&((type*)0)->member)))

#define container_of(ptr, type, member) \
    (type *)((char *)ptr - cc_offsetof(type, member))

template<typename T>
class ListNode {
public:
    ListNode() {
        next_ = nullptr;
        prev_ = nullptr;
    }
    ListNode(ListNode<T> *next, ListNode<T> *prev) {
        next_ = next;
        prev_ = prev;
    }
    ~ListNode() {}
    ListNode(const ListNode<T> &) = delete;
    ListNode<T> & operator=(const ListNode<T> &) = delete;

    void InsertBefore(ListNode<T> *node) {
        node->next_ = this;
        node->prev_ = prev_;
        prev_->next_ = node;
        prev_ = node;
    }
    void InsertAfer(ListNode<T> *node) {
        node->next_ = next_;
        node->prev_ = this;
        next_->prev_ = node;
        next_ = node;
    }
    void Remove() {
        prev_->next_ = next_;
        next_->prev_ = prev_;
        next_ = nullptr;
        prev_ = nullptr;
    }
    ListNode<T> *next() const {
        return next_;
    }
    ListNode<T> *prev() const {
        return prev_;
    }
    const T *value() const {
        return static_cast<T *>(this);
    }
    T *value() {
        return static_cast<T *>(this);
    }

private:
    ListNode<T> *next_;
    ListNode<T> *prev_;
};


template<typename T>
class DList {
public:
    DList() : root_(&root_, &root_) {
    }
    ~DList() {
        Clear();
    }

    DList(const DList &) = delete;
    DList& operator=(const DList &) = delete;

    void Append(ListNode<T> *node) {
        root_.InsertBefore(node);
    }
    void Prepend(ListNode<T> *node) {
        root_.InsertAfer(node);
    }
    void Remove(ListNode<T> *node) {
        node->Remove();
    }
    void IterateSafe(std::function<void(T *)> fn) {
        ListNode<T> *curr, *next;
        for (curr = head(), next = curr->next();
             curr != end();
             curr = next, next = curr->next()) {
            fn(curr->value());
        }
    }
    void Iterate(std::function<void(T *)> fn) {
        for (ListNode<T> *curr = head(); curr != end();
             curr = curr->next()) {
            fn(curr->value());
        }
    }
    void Clear() {
        IterateSafe([](T *n) {
            n->Remove();
        });
    }
    ListNode<T> *head() const {
        return root_.next();
    }
    ListNode<T> *tail() const {
        return root_.prev();
    }
    const ListNode<T> *end() const {
        return &root_;
    }
    bool empty() const {
        return head() == end();
    }

private:
    ListNode<T> root_;
};

#endif //DLIST_H_
