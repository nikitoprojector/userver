#include <components/minimal_component_list.hpp>
#include <components/minimal_server_component_list.hpp>

#include <server/component.hpp>
#include <server/handlers/auth/auth_checker_settings_component.hpp>
#include <server_settings/http_server_settings_component.hpp>

namespace components {

ComponentList MinimalServerComponentList() {
  return components::MinimalComponentList()
      .Append<components::HttpServerSettings<>>()
      .Append<components::Server>()
      .Append<components::AuthCheckerSettings>();
}

}  // namespace components