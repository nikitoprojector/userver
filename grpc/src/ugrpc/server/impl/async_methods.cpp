#include <userver/ugrpc/server/impl/async_methods.hpp>

#include <userver/engine/task/cancel.hpp>
#include <userver/logging/log.hpp>

USERVER_NAMESPACE_BEGIN

namespace ugrpc::server::impl {

void ReportErrorWhileCancelling(std::string_view call_name) noexcept {
  LOG_ERROR() << "Connection error while cancelling call '" << call_name << "'";
}

impl::AsyncMethodInvocation::WaitStatus Wait(
    impl::AsyncMethodInvocation& async) {
  engine::TaskCancellationBlocker blocker;
  const auto status = async.Wait();
  UASSERT(status != impl::AsyncMethodInvocation::WaitStatus::kCancelled);
  return status;
}

void ThrowOnError(impl::AsyncMethodInvocation::WaitStatus status,
                  std::string_view call_name, std::string_view stage_name) {
  switch (status) {
    case AsyncMethodInvocation::WaitStatus::kOk:
      return;

    case AsyncMethodInvocation::WaitStatus::kError:
      throw RpcInterruptedError(call_name, stage_name);

    case AsyncMethodInvocation::WaitStatus::kCancelled:
      UINVARIANT(false, "Cancel happened on server operation");
  }
}

const grpc::Status kUnimplementedStatus{grpc::StatusCode::UNIMPLEMENTED,
                                        "This method is unimplemented"};

const grpc::Status kUnknownErrorStatus{
    grpc::StatusCode::UNKNOWN,
    "The service method has exited unexpectedly, without providing a status"};

}  // namespace ugrpc::server::impl

USERVER_NAMESPACE_END
