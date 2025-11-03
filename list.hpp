#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        /**
         * add data members and constructors & destructor
         */
        node *prev;
        node *next;
        T *val; // nullptr for sentinel nodes

        node(): prev(nullptr), next(nullptr), val(nullptr) {}
        explicit node(const T &value): prev(nullptr), next(nullptr), val(new T(value)) {}
        ~node(){ if (val) { delete val; val = nullptr; } }
    };

protected:
    /**
     * add data members for linked list as protected members
     */
    node *head; // sentinel head (no value)
    node *tail; // sentinel tail (no value)
    size_t sz;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        // link: prev <-> cur <-> pos
        node *prev = pos->prev;
        cur->next = pos;
        cur->prev = prev;
        pos->prev = cur;
        if (prev) prev->next = cur; else head = cur; // should not happen for sentinel usage
        ++sz;
        return cur;
    }
    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        node *p = pos->prev;
        node *n = pos->next;
        if (p) p->next = n; else head = n; // should not happen for sentinel usage
        if (n) n->prev = p; else tail = p; // should not happen for sentinel usage
        pos->prev = pos->next = nullptr;
        --sz;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        node *cur;
        const list<T> *owner;
    public:
        iterator(): cur(nullptr), owner(nullptr) {}
        iterator(const list<T> *o, node *c): cur(c), owner(o) {}
        /**
         * iter++
         */
        iterator operator++(int) {
            iterator tmp = *this;
            if (owner == nullptr || cur == nullptr || cur == owner->tail) throw invalid_iterator();
            cur = cur->next;
            return tmp;
        }
        /**
         * ++iter
         */
        iterator & operator++() {
            if (owner == nullptr || cur == nullptr || cur == owner->tail) throw invalid_iterator();
            cur = cur->next;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            iterator tmp = *this;
            if (owner == nullptr || cur == nullptr) throw invalid_iterator();
            // decrement from begin() is invalid; decrement from end() allowed only if list not empty
            if (cur == owner->head) throw invalid_iterator();
            if (cur == owner->tail) {
                if (owner->sz == 0) throw invalid_iterator();
                cur = owner->tail->prev;
                return tmp;
            }
            if (cur->prev == owner->head) {
                // would point to head sentinel -> invalid
                throw invalid_iterator();
            }
            cur = cur->prev;
            return tmp;
        }
        /**
         * --iter
         */
        iterator & operator--() {
            if (owner == nullptr || cur == nullptr) throw invalid_iterator();
            if (cur == owner->head) throw invalid_iterator();
            if (cur == owner->tail) {
                if (owner->sz == 0) throw invalid_iterator();
                cur = owner->tail->prev;
                return *this;
            }
            if (cur->prev == owner->head) throw invalid_iterator();
            cur = cur->prev;
            return *this;
        }
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (owner == nullptr || cur == nullptr || cur->val == nullptr) throw invalid_iterator();
            return *(cur->val);
        }
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (owner == nullptr || cur == nullptr || cur->val == nullptr) throw invalid_iterator();
            return cur->val;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const { return cur == rhs.cur; }
        bool operator==(const const_iterator &rhs) const { return cur == rhs.cur; }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
        bool operator!=(const const_iterator &rhs) const { return cur != rhs.cur; }

        friend class list<T>;
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        node *cur;
        const list<T> *owner;
    public:
        const_iterator(): cur(nullptr), owner(nullptr) {}
        const_iterator(const list<T> *o, node *c): cur(c), owner(o) {}
        const_iterator(const iterator &it): cur(it.cur), owner(it.owner) {}
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            if (owner == nullptr || cur == nullptr || cur == owner->tail) throw invalid_iterator();
            cur = cur->next;
            return tmp;
        }
        const_iterator & operator++() {
            if (owner == nullptr || cur == nullptr || cur == owner->tail) throw invalid_iterator();
            cur = cur->next;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            if (owner == nullptr || cur == nullptr) throw invalid_iterator();
            if (cur == owner->head) throw invalid_iterator();
            if (cur == owner->tail) {
                if (owner->sz == 0) throw invalid_iterator();
                cur = owner->tail->prev;
                return tmp;
            }
            if (cur->prev == owner->head) throw invalid_iterator();
            cur = cur->prev;
            return tmp;
        }
        const_iterator & operator--() {
            if (owner == nullptr || cur == nullptr) throw invalid_iterator();
            if (cur == owner->head) throw invalid_iterator();
            if (cur == owner->tail) {
                if (owner->sz == 0) throw invalid_iterator();
                cur = owner->tail->prev;
                return *this;
            }
            if (cur->prev == owner->head) throw invalid_iterator();
            cur = cur->prev;
            return *this;
        }
        const T & operator *() const {
            if (owner == nullptr || cur == nullptr || cur->val == nullptr) throw invalid_iterator();
            return *(cur->val);
        }
        const T * operator ->() const {
            if (owner == nullptr || cur == nullptr || cur->val == nullptr) throw invalid_iterator();
            return cur->val;
        }
        bool operator==(const const_iterator &rhs) const { return cur == rhs.cur; }
        bool operator==(const iterator &rhs) const { return cur == rhs.cur; }
        bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
        bool operator!=(const iterator &rhs) const { return !(*this == rhs); }

        friend class list<T>;
    };
    
    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() {
        head = new node();
        tail = new node();
        head->next = tail; head->prev = nullptr;
        tail->prev = head; tail->next = nullptr;
        sz = 0;
    }
    list(const list &other) {
        head = new node();
        tail = new node();
        head->next = tail; head->prev = nullptr;
        tail->prev = head; tail->next = nullptr;
        sz = 0;
        for (node *p = other.head->next; p != other.tail; p = p->next) {
            push_back(*(p->val));
        }
    }
    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
        delete head; head = nullptr;
        delete tail; tail = nullptr;
    }
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) return *this;
        clear();
        for (node *p = other.head->next; p != other.tail; p = p->next) {
            push_back(*(p->val));
        }
        return *this;
    }
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (sz == 0) throw container_is_empty();
        return *(head->next->val);
    }
    const T & back() const {
        if (sz == 0) throw container_is_empty();
        return *(tail->prev->val);
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() { return iterator(this, sz ? head->next : tail); }
    const_iterator cbegin() const { return const_iterator(this, sz ? head->next : tail); }
    /**
     * returns an iterator to the end.
     */
    iterator end() { return iterator(this, tail); }
    const_iterator cend() const { return const_iterator(this, tail); }
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const { return sz == 0; }
    /**
     * returns the number of elements
     */
    virtual size_t size() const { return sz; }

    /**
     * clears the contents
     */
    virtual void clear() {
        node *p = head->next;
        while (p != tail) {
            node *n = p->next;
            delete p;
            p = n;
        }
        head->next = tail;
        tail->prev = head;
        sz = 0;
    }
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.owner != this || pos.cur == nullptr) throw invalid_iterator();
        node *p = pos.cur;
        node *nd = new node(value);
        // insert before p
        nd->prev = p->prev;
        nd->next = p;
        p->prev->next = nd;
        p->prev = nd;
        ++sz;
        return iterator(this, nd);
    }
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (pos.owner != this || pos.cur == nullptr) throw invalid_iterator();
        if (sz == 0) throw container_is_empty();
        node *p = pos.cur;
        if (p == tail || p->val == nullptr) throw invalid_iterator();
        node *nxt = p->next;
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
        --sz;
        return iterator(this, nxt);
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) { insert(end(), value); }
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (sz == 0) throw container_is_empty();
        iterator it(this, tail->prev);
        erase(it);
    }
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) { insert(begin(), value); }
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (sz == 0) throw container_is_empty();
        iterator it(this, head->next);
        erase(it);
    }
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (sz <= 1) return;
        node **arr = new node*[sz];
        size_t i = 0;
        for (node *p = head->next; p != tail; p = p->next) arr[i++] = p;
        sjtu::sort<node*>(arr, arr + sz, [](node *const &a, node *const &b){ return *(a->val) < *(b->val); });
        head->next = arr[0];
        arr[0]->prev = head;
        for (size_t k = 1; k < sz; ++k) {
            arr[k-1]->next = arr[k];
            arr[k]->prev = arr[k-1];
        }
        arr[sz-1]->next = tail;
        tail->prev = arr[sz-1];
        delete [] arr;
    }
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other || other.sz == 0) return;
        node *p1 = head->next;
        node *p2 = other.head->next;
        while (p1 != tail && p2 != other.tail) {
            if (*(p2->val) < *(p1->val)) {
                // detach p2 from other
                node *n2 = p2->next;
                p2->prev->next = p2->next;
                p2->next->prev = p2->prev;
                // insert p2 before p1
                p2->prev = p1->prev;
                p2->next = p1;
                p1->prev->next = p2;
                p1->prev = p2;
                // advance p2
                p2 = n2;
                ++sz; --other.sz;
            } else {
                p1 = p1->next;
            }
        }
        if (p2 != other.tail) {
            // splice remaining [p2, other.tail->prev] to before tail
            node *first = p2;
            node *last = other.tail->prev;
            // detach from other
            other.head->next = other.tail;
            other.tail->prev = other.head;
            size_t moved = 0;
            for (node *q = first; ; q = q->next) { ++moved; if (q == last) break; }
            other.sz -= moved; sz += moved;
            // attach to this
            last->next = tail;
            first->prev = tail->prev;
            tail->prev->next = first;
            tail->prev = last;
        }
    }
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (sz <= 1) return;
        node *l = head->next;
        node *r = tail->prev;
        for (size_t i = 0; i < sz / 2; ++i) {
            T *tmp = l->val;
            l->val = r->val;
            r->val = tmp;
            l = l->next;
            r = r->prev;
        }
    }
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (sz <= 1) return;
        node *p = head->next;
        while (p != tail) {
            node *n = p->next;
            while (n != tail && !(*(p->val) != *(n->val))) {
                node *del = n;
                n = n->next;
                // unlink del
                del->prev->next = del->next;
                del->next->prev = del->prev;
                delete del;
                --sz;
            }
            p = n;
        }
    }
};

}

#endif //SJTU_LIST_HPP
