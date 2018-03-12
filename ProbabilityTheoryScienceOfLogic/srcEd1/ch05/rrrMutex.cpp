/*
 * =========================================================================
 * Copyright (c) 2018, Raymon White - All Rights Reserved
 * Author:  Raymon White (ray), rayaxiom42 AT gmail DOT com
 * =========================================================================
 */
#include<iostream>
#include<map>
#include<string>
#include<chrono>
#include<thread>
#include<mutex>

std::map<std::string, std::string> g_pages;
std::mutex g_pages_mutex;

void save_page(const std::string &url)
{
  // simulate a long page fetch
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::string result = "fake content";

  // now use the lock to access the shared map
  std::lock_guard<std::mutex> guard(g_pages_mutex);
  g_pages[url] = result;
}

int main()
{
  std::thread t1(save_page, "http://foo");
  std::thread t2(save_page, "http://bar");

  t1.join();
  t2.join();

  // safe to access g_pages without lock now, as the threads are joined
  for(const auto& pair : g_pages)
  {
    std::cout << pair.first << " => " << pair.second << '\n';
  }
  return 0;
}

