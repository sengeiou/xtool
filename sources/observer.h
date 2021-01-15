#ifndef OBSERVER_H_
#define OBSERVER_H_

#include "dlist.h"

class ObserverBase : public ListNode<ObserverBase> {
public:
    Observer() : ListNode() {}
    virtual ~Observer() {}
    virtual void Update(void *) = 0;
};

class ObserverList {
public:
    ObserverList() : list_obs_() {

    }
    ~ObserverList() {

    }
    void AddObserver(ObserverBase *obs) {
        list_obs_.Append(obs);
    }
    void RemoveObserver(ObserverBase *obs) {
        list_obs_.Remove(obs);
    }
    void Notify() {
        ObserverBase *obs;
        for (ListNode *curr = list_obs_.head();
             curr != list_obs_.end();
             curr = curr->next()) {
            obs = static_cast<ObserverBase *>(curr);
            obs->Update();
        }
    }

private:
    DList<ObserverBase> list_obs_;
};

#endif //OBSERVER_H_
