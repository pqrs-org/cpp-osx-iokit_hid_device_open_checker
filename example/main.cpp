#include <csignal>
#include <pqrs/osx/iokit_hid_device_open_checker.hpp>

namespace {
auto global_wait = pqrs::make_thread_wait();
}

int main(void) {
  std::signal(SIGINT, [](int) {
    global_wait->notify();
  });

  auto time_source = std::make_shared<pqrs::dispatcher::hardware_time_source>();
  auto dispatcher = std::make_shared<pqrs::dispatcher::dispatcher>(time_source);

  std::vector<pqrs::cf::cf_ptr<CFDictionaryRef>> matching_dictionaries{
      pqrs::osx::iokit_hid_manager::make_matching_dictionary(
          pqrs::hid::usage_page::generic_desktop,
          pqrs::hid::usage::generic_desktop::keyboard),
  };

  auto checker = std::make_unique<pqrs::osx::iokit_hid_device_open_checker>(dispatcher,
                                                                            matching_dictionaries);

  checker->device_open_permitted.connect([&] {
    std::cout << "device_open_permitted" << std::endl;
    global_wait->notify();
  });

  checker->device_open_forbidden.connect([&] {
    std::cout << "device_open_forbidden" << std::endl;
    global_wait->notify();
  });

  checker->async_start();

  // ============================================================

  global_wait->wait_notice();

  // ============================================================

  checker = nullptr;

  dispatcher->terminate();
  dispatcher = nullptr;

  std::cout << "finished" << std::endl;

  return 0;
}
