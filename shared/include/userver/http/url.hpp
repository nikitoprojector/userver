#pragma once

/// @file userver/http/url.hpp
/// @brief URL manipulation functions

#include <string>
#include <string_view>
#include <unordered_map>

namespace http {

/// @brief Decode URL
[[deprecated("Use a more strict http::parser::UrlDecode instead")]] std::string
UrlDecode(std::string_view range);

/// @brief Encode as URL
std::string UrlEncode(std::string_view input_string);

using Args = std::unordered_map<std::string, std::string>;

/// @brief Make an URL query
std::string MakeQuery(const Args& query_args);

/// @brief Make an URL query
std::string MakeQuery(
    std::initializer_list<std::pair<std::string_view, std::string_view>>
        query_args);

/// @brief Make an URL with query arguments
std::string MakeUrl(std::string_view path, const Args& query_args);

/// @brief Make an URL with query arguments
std::string MakeUrl(
    std::string_view path,
    std::initializer_list<std::pair<std::string_view, std::string_view>>
        query_args);

/// @brief Returns URL part before the first '?' character
std::string ExtractMetaTypeFromUrl(const std::string& url);

}  // namespace http