#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include<exception>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a
 * list.
 */
template <typename T> class list {
protected:
  class node {
  public:
    /**
     * add data members and constructors & destructor
     */
    T data;
    node *prev,*next;

    //node():prev(nullptr),next(nullptr){}
    node(const T &data,node *prev=nullptr,node *next=nullptr):data(data),prev(prev),next(next){}
    /*node(const node &x){
      data=x.data;
      prev=x.prev;
      next=x.next;
    }*/
  };

protected:
  /**
   * add data members for linked list as protected members
   */
  node *head,*tail;
  int list_size;
  
  /**
   * insert node cur before node pos
   * return the inserted node cur
   */
  node *insert(node *pos, node *cur) {
    if(head==pos){
      cur->next=head;
      head->prev=cur;
      head=cur;
    }
    else{
      node *nw=pos->prev;
      if(nw)
        nw->next=cur;
      cur->prev=nw;
      cur->next=pos;
      pos->prev=cur;
    }
    list_size++;
    return cur;
  }
  /**
   * remove node pos from list (no need to delete the node)
   * return the removed node pos
   */
  node *erase(node *pos) {
    if(head==pos){
      head=head->next;
      if(head)
        head->prev=nullptr;
    }
    else if(tail==pos){
      tail=tail->prev;
      if(tail)
        tail->next=nullptr;
    }
    else{
      node *nw=pos->prev;
      nw->next=pos->next;
      pos->next->prev=nw;
      pos->next=nullptr;
      pos->prev=nullptr;
    }
    list_size--;
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
    
  public:
    node *current;
    bool is_end;
    iterator():current(nullptr),is_end(true){}
    iterator(node *ptr,bool is_end=false):current(ptr),is_end(is_end){}
    /*~iterator(){
      delete current;
    }*/
    iterator operator++(int) {
      iterator tmp=*this;
      ++(*this);
      return tmp;
    }
    iterator &operator++() {
      if(current){
        if(current->next==nullptr)
          is_end=true;
        else if(current)
          current=current->next;
      }
      return *this;
    }
    iterator operator--(int) {
      iterator tmp=*this;
      --(*this);
      return tmp;
    }
    iterator &operator--() {
      if(is_end)
        is_end=false;
      else if(current)
        current=current->prev;
      return *this;
    }

    /**
     * TODO *it
     * throw std::exception if iterator is invalid
     */
    T &operator*() const {
      if(current==nullptr||is_end)
        throw std::exception();
      return current->data;
    }
    /**
     * TODO it->field
     * throw std::exception if iterator is invalid
     */
    T *operator->() const noexcept {
      if(current==nullptr||is_end)
        throw std::exception();
      return &(current->data);
    }

    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory).
     */
    bool operator==(const iterator &rhs) const {
      if(current==rhs.current&&is_end==rhs.is_end)
        return true;
      else
        return false;
    }
    bool operator==(const const_iterator &rhs) const {
      if(current==rhs.current&&is_end==rhs.is_end)
        return true;
      else
        return false;
    }
    
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
      if(current!=rhs.current||is_end!=rhs.is_end)
        return true;
      else
        return false;
    }
    bool operator!=(const const_iterator &rhs) const {
      if(current!=rhs.current||is_end!=rhs.is_end)
        return true;
      else
        return false;
    }
  };

  /**
   * TODO
   * has same function as iterator, just for a const object.
   * should be able to construct from an iterator.
   */
  class const_iterator {
    private:
      node* current;
      bool is_end;
    public:
      const_iterator():current(nullptr),is_end(true){}
      const_iterator(node *ptr,bool is_end=false):current(ptr),is_end(is_end){}
      const_iterator(const iterator& other):current(other.current),is_end(other.is_end){}
      /*~const_iterator(){
        delete current;
      }*/
      const_iterator operator++(int) {
        iterator tmp=*this;
        ++(*this);
        return tmp;
      }
      const_iterator &operator++() {
        if(current){
          if(current->next==nullptr)
            is_end=true;
          else if(current)
            current=current->next;
        }
        return *this;
      }
      const_iterator operator--(int) {
        iterator tmp=*this;
        --(*this);
        return tmp;
      }
      const_iterator &operator--() {
        if(is_end)
          is_end=false;
        else if(current)
          current=current->prev;
        return *this;
      }

    /**
     * TODO *it
     * throw std::exception if iterator is invalid
     */
      const T &operator*() const {
        if(current==nullptr)
          throw std::exception();
        return current->data;
      }
    /**
     * TODO it->field
     * throw std::exception if iterator is invalid
     */
      const T *operator->() const noexcept {
        if(current==nullptr)
          throw std::exception();
        return &(current->data);
      }

    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory).
     */
    bool operator==(const iterator &rhs) const {
      if(current==rhs.current&&is_end==rhs.is_end)
        return true;
      else
        return false;
    }
    bool operator==(const const_iterator &rhs) const {
      if(current==rhs.current&&is_end==rhs.is_end)
        return true;
      else
        return false;
    }
    
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
      if(current!=rhs.current||is_end!=rhs.is_end)
        return true;
      else
        return false;
    }
    bool operator!=(const const_iterator &rhs) const {
      if(current!=rhs.current||is_end!=rhs.is_end)
        return true;
      else
        return false;
    }
  };

  /**
   * TODO Constructs
   * Atleast two: default constructor, copy constructor
   */
  list():head(nullptr),tail(nullptr),list_size(0){}
  void copy(const list &other){
    head=nullptr;
    tail=nullptr;
    list_size=0;
    if(other.list_size){
      head=tail=new node(other.head->data);
      node *nxt=other.head->next;
      while(nxt!=nullptr){
        tail->next=new node(nxt->data,tail);
        tail=tail->next;
        nxt=nxt->next;
      }
      list_size=other.list_size;
    }
  }
  list(const list &other) {
    copy(other);
  }
  /**
   * TODO Destructor
   */
  virtual ~list() {
    clear();
  }
  /**
   * TODO Assignment operator
   */
  list &operator=(const list &other) {
    if(this==&other)
      return *this;
    clear();
    copy(other);
    return *this;
  }
  /**
   * access the first / last element
   * throw container_is_empty when the container is empty.
   */
  const T &front() const {
    if(list_size==0)
      throw std::exception();
    return head->data;
  }
  const T &back() const {
    if(list_size==0)
      throw std::exception();
    return tail->data;
  }
  /**
   * returns an iterator to the beginning.
   */
  iterator begin() {
    return iterator(head,head==nullptr);
  }
  const_iterator cbegin() const {
    return const_iterator(head,head==nullptr);
  }
  /**
   * returns an iterator to the end.
   */
  iterator end() {
    return iterator(tail,true);
  }
  const_iterator cend() const {
    return const_iterator(tail,true);
  }
  /**
   * checks whether the container is empty.
   */
  virtual bool empty() const {
    if(list_size==0)
      return true;
    else
      return false;
  }
  /**
   * returns the number of elements
   */
  virtual size_t size() const {
    return list_size;
  }

  /**
   * clears the contents
   */
  virtual void clear() {
    node* current = head;
    while (current != nullptr) {
      node* next = current->next;
      delete current;
      current = next;
    }
    head = nullptr;
    tail = nullptr;
    list_size = 0;
  }
  /**
   * insert value before pos (pos may be the end() iterator)
   * return an iterator pointing to the inserted value
   * throw if the iterator is invalid
   */
  virtual iterator insert(iterator pos, const T &value) {
    list_size++;
    if(list_size==1){
      node *nw=new node(value);
      head=tail=nw;
      return iterator(nw);
    }
    else if(pos.is_end){
      node *nw=new node(value);
      nw->prev=tail;
      tail->next=nw;
      tail=nw;
      return iterator(nw);
    }
    else if(pos.current==head){
      node *nw=new node(value);
      nw->next=head;
      head->prev=nw;
      head=nw;
      return iterator(nw);
    }
    else{
      node *nw=new node(value);
      nw->next=head;
      node *ptr=pos.current->prev;
      ptr->next=nw;
      nw->next=pos.current;
      nw->prev=ptr;
      pos.current->prev=nw;
      return iterator(nw);
    }
  }
  /**
   * remove the element at pos (the end() iterator is invalid)
   * returns an iterator pointing to the following element, if pos pointing to
   * the last element, end() will be returned. throw if the container is empty,
   * the iterator is invalid
   */
  virtual iterator erase(iterator pos) {
    if(pos.current==nullptr||pos.is_end)
      throw std::exception();
    node* nxt=pos.current->next;
    if(pos.current==head){
      head=nxt;
      pos.current->next=nullptr;
      if(list_size==1)
        tail=nullptr;
      if(head)
        head->prev=nullptr;
    }
    else if(pos.current==tail){
      tail=pos.current->prev;
      pos.current->prev=nullptr;
      if(list_size==1)
        head=nullptr;
      if(tail)
        tail->next=nullptr;
    }
    else{
      node* prv=pos.current->prev;
      prv->next=nxt;
      nxt->prev=prv;
      pos.current->prev=nullptr;
      pos.current->next=nullptr;
    }
    delete pos.current;
    list_size--;
    return iterator(nxt);
  }
  /**
   * adds an element to the end
   */
  void push_back(const T &value) {
    if(list_size==0){
      node *nw=new node(value);
      head=nw;
      tail=nw;
    }
    else{
      node *nw=new node(value);
      nw->prev=tail;
      tail->next=nw;
      tail=nw;
    }
    list_size++;
  }
  /**
   * removes the last element
   * throw when the container is empty.
   */
  void pop_back() {
    if(list_size==0)
      throw std::exception();
    if(list_size==1){
      delete tail;
      head=tail=nullptr;
    }
    else{
      node* tmp=tail;
      tail=tail->prev;
      tail->next=nullptr;
      delete tmp;
    }
    list_size--;
  }
  /**
   * inserts an element to the beginning.
   */
  void push_front(const T &value) {
    node *nw=new node(value);
    nw->next=head;
    if(head)
      head->prev=nw;
    else
      tail=nw;
    head=nw;
    list_size++;
  }
  /**
   * removes the first element.
   * throw when the container is empty.
   */
  void pop_front() {
    if(list_size==0)
      throw std::exception();
    if(list_size==1){
      delete head;
      head=tail=nullptr;
    }
    else{
      node* tmp=head;
      head=head->next;
      head->prev=nullptr;
      delete tmp;
    }
    list_size--;
  }
};

} // namespace sjtu

#endif // SJTU_LIST_HPP