#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

#include <userver/formats/json/value.hpp>
#include <userver/taxi_config/snapshot.hpp>
#include <userver/yaml_config/yaml_config.hpp>

namespace dump {

namespace impl {
std::chrono::milliseconds ParseMs(
    const formats::json::Value& value,
    std::optional<std::chrono::milliseconds> default_value = {});
}

extern const std::string_view kDump;
extern const std::string_view kMaxDumpAge;

struct ConfigPatch final {
  std::optional<bool> dumps_enabled;
  std::optional<std::chrono::milliseconds> min_dump_interval;
};

ConfigPatch Parse(const formats::json::Value& value,
                  formats::parse::To<ConfigPatch>);

struct Config final {
  Config(std::string name, const yaml_config::YamlConfig& config,
         std::string_view dump_root);

  Config MergeWith(const ConfigPatch& patch) const;

  std::string name;
  uint64_t dump_format_version;
  bool world_readable;
  std::string dump_directory;
  std::string fs_task_processor;
  uint64_t max_dump_count;
  std::optional<std::chrono::milliseconds> max_dump_age;
  bool max_dump_age_set;
  bool dump_is_encrypted;

  bool dumps_enabled;
  std::chrono::milliseconds min_dump_interval;
};

std::unordered_map<std::string, ConfigPatch> ParseConfigSet(
    const taxi_config::DocsMap& docs_map);

inline constexpr taxi_config::Key<ParseConfigSet> kConfigSet;

}  // namespace dump