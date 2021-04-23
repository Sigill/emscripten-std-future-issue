#include <chrono>
#include <future>
#include <thread>
#include <vector>
#include <iostream>

#include <emscripten/bind.h>

void work(int i) {
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void multithreaded_step() {
  std::cout << __FUNCTION__ << " start" << std::endl;
  unsigned int nbThread = 8;
  std::vector<std::thread> threads;

  for(int i = 0; i < nbThread; ++i) {
    threads.emplace_back(work, i);
  }
  std::cout << __FUNCTION__ << " threads created " << std::endl;

  for(size_t i = 0; i < threads.size(); ++i) {
    threads[i].join();
  }

  std::cout << __FUNCTION__ << " end" << std::endl;
}

void multithreaded_workload() {
  std::cout << __FUNCTION__ << " start" << std::endl;
  multithreaded_step();
  multithreaded_step();
  multithreaded_step();
  std::cout << __FUNCTION__ << " end" << std::endl;
}

std::future<void> multithreaded_workload_async() {
  return std::async(std::launch::async, &multithreaded_workload);
}

bool try_get(const std::future<void>& f) {
  auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(1);
  return f.wait_until(deadline) != std::future_status::timeout;
}

EMSCRIPTEN_BINDINGS(my_module)
{
  emscripten::class_<std::future<void>>("std_future_void")
    .function("get", &std::future<void>::get)
    .function("try_get", &try_get);

  emscripten::function("multithreaded_workload", &multithreaded_workload);
  emscripten::function("multithreaded_workload_async", &multithreaded_workload_async);
}