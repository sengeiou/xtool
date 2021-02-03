#ifndef OBSERVER_H_
#define OBSERVER_H_

#include "dlist.h"

class ObserverBase : public ListNode<ObserverBase> {
public:
    ObserverBase(): ListNode<ObserverBase>() {}
    virtual ~ObserverBase() {}
    virtual void Update(int action, void *) = 0;
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
    void Notify(int action, void *ptr) {
        for (ListNode<ObserverBase> *curr = list_obs_.head();
             curr != list_obs_.end();
             curr = curr->next()) {
            ObserverBase *obs = curr->value();
            obs->Update(action, ptr);
        }
    }

private:
    DList<ObserverBase> list_obs_;
};

#endif //OBSERVER_H_
