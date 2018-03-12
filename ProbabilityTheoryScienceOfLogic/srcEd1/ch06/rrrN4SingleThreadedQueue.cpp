/*
 * =========================================================================
 * Copyright (c) 2018, Raymon White - All Rights Reserved
 * Author:  Raymon White (ray), rayaxiom42 AT gmail DOT com
 * =========================================================================
 */
#include<iostream>
#include<memory>

template<typename T>
class queue
{
private:
  struct node
  {
    T data;
    std::unique_ptr<node> next;

    node(T data_):
      data(std::move(data_))
    {}
  };

  std::unique_ptr<node> head;         //1
  node* tail;                         //2

public:
  queue()
  {}

  queue(const queue& other) = delete;
  queue& operator=(const queue& other) = delete;

  std::shared_ptr<T> try_pop()
  {
    if(!head)
    {
      return std::shared_ptr<T>{};
    }

    std::shared_ptr<T> const res(
        std::make_shared<T>(std::move(head->data)));

    std::unique_ptr<node> const old_head = std::move(head);     //3
    head = std::move(old_head->next);
    return res;
  }

  void push(T new_value)
  {
    std::unique_ptr<node> p(new node(std::move(new_value)));
    node* const new_tail=p.get();

    if(tail)
    {
      tail->next = std::move(p);      //4
    }
    else
    {
      head = std::move(p);            //5
    }
    tail = new_tail;                  //6
  }
};





int main()
{
  return 0;
}
















