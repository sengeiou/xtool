#ifndef DLIST_H_
#define DLIST_H_

#include <cstddef>

#define cc_offsetof(type, member) \
    ((size_t)((char*)(&((type*)0)->member)))

#define container_of(ptr, type, member) \
    (type *)((char *)ptr - cc_offsetof(type, member))

class ListNode {
public:
    ListNode() {
        next_ = nullptr;
        prev_ = nullptr;
    }
    ListNode(ListNode *next, ListNode *prev) {
        next_ = next;
        prev_ = prev;
    }
    ~ListNode() {}
    ListNode(const ListNode &) = delete;
    ListNode & operator=(const ListNode &) = delete;

    void InsertBefore(ListNode *node) {
        node->next_ = this;
        node->prev_ = prev_;
        prev_->next_ = node;
        prev_ = node;
    }
    void InsertAfer(ListNode *node) {
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
    ListNode *next() {
        return next_;
    }
    ListNode *prev() {
        return prev_;
    }

private:
    ListNode *next_;
    ListNode *prev_;
};

class DList {
public:
    DList() : root_(&root_, &root_) {
    }
    ~DList() {
        Clear();
    }

    DList(const DList &) = delete;
    DList& operator=(const DList &) = delete;

    void Append(ListNode *node) {
        root_.InsertBefore(node);
    }
    void Prepend(ListNode *node) {
        root_.InsertAfer(node);
    }
    void Remove(ListNode *node) {
        node->Remove();
    }
    void Clear() {
        ListNode *curr, *next;
        for (curr = head(), next = curr->next();
             curr != end();
             curr = next, next = curr->next()) {
            Remove(curr);
        }
    }
    ListNode *head() {
        return root_.next();
    }
    ListNode *tail() {
        return root_.prev();
    }
    ListNode *end() {
        return &root_;
    }
    bool empty() {
        return head() == end();
    }

private:
    ListNode root_;
};


#endif //DLIST_H_
