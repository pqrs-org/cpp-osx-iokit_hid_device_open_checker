#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <pqrs/osx/iokit_hid_device_open_checker.hpp>

TEST_CASE("iokit_hid_device_open_checker") {
  auto time_source = std::make_shared<pqrs::dispatcher::hardware_time_source>();
  auto dispatcher = std::make_shared<pqrs::dispatcher::dispatcher>(time_source);

  std::vector<pqrs::cf::cf_ptr<CFDictionaryRef>> matching_dictionaries{
      pqrs::osx::iokit_hid_manager::make_matching_dictionary(
          pqrs::hid::usage_page::generic_desktop,
          pqrs::hid::usage::generic_desktop::keyboard),
  };

  auto checker = std::make_unique<pqrs::osx::iokit_hid_device_open_checker>(dispatcher,
                                                                            matching_dictionaries);

  checker->async_start();

  checker = nullptr;

  dispatcher->terminate();
  dispatcher = nullptr;
}
