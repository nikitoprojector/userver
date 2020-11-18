#include <server/http/form_data_arg.hpp>

#include <sstream>

#include <http/content_type.hpp>

namespace server::http {

std::string FormDataArg::Charset() const {
  static const std::string kDefaultCharset = "UTF-8";
  if (content_type) {
    ::http::ContentType parsed_content_type(*content_type);
    if (parsed_content_type.HasExplicitCharset())
      return parsed_content_type.Charset();
  }
  if (default_charset)
    return *default_charset;
  else
    return kDefaultCharset;
}

std::string FormDataArg::ToDebugString() const {
  std::ostringstream os;
  os << "value=" << value;
  os << ", content_disposition='" << content_disposition << '\'';
  if (filename) os << ", filename=" << *filename;
  if (content_type) os << ", content_type=" << *content_type;
  os << ", charset=" << Charset();
  return os.str();
}

}  // namespace server::http