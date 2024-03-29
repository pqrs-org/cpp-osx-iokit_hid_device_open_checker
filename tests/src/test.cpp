#include <boost/ut.hpp>
#include <pqrs/osx/iokit_hid_device_open_checker.hpp>

int main(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "iokit_hid_device_open_checker"_test = [] {
    auto time_source = std::make_shared<pqrs::dispatcher::hardware_time_source>();
    auto dispatcher = std::make_shared<pqrs::dispatcher::dispatcher>(time_source);
    auto run_loop_thread = std::make_shared<pqrs::cf::run_loop_thread>();

    std::vector<pqrs::cf::cf_ptr<CFDictionaryRef>> matching_dictionaries{
        pqrs::osx::iokit_hid_manager::make_matching_dictionary(
            pqrs::hid::usage_page::generic_desktop,
            pqrs::hid::usage::generic_desktop::keyboard),
    };

    auto checker = std::make_unique<pqrs::osx::iokit_hid_device_open_checker>(dispatcher,
                                                                              run_loop_thread,
                                                                              matching_dictionaries);

    checker->async_start();

    checker = nullptr;

    run_loop_thread->terminate();
    run_loop_thread = nullptr;

    dispatcher->terminate();
    dispatcher = nullptr;
  };

  return 0;
}
