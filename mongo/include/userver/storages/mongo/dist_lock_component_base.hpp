#pragma once

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/dist_lock/dist_locked_worker.hpp>
#include <userver/storages/mongo/collection.hpp>
#include <userver/storages/mongo/dist_lock_strategy.hpp>
#include <userver/utils/statistics/storage.hpp>

namespace storages::mongo {

// clang-format off

/// @ingroup userver_base_classes userver_components
///
/// @brief Base class for mongo-based distlock worker components
///
/// A component that implements a distlock with lock in Mongo. Inherit from
/// DistLockComponentBase and implement DoWork(). Lock options are configured
/// in static config.
///
/// Mongo might not perform well on a high load, so you might want to use
/// postgres-based distlock storages::postgres::DistLockComponentBase instead.
/// @see storages::postgres::DistLockComponentBase
///
/// ## Static configuration example:
///
/// ```yaml
///        example-distlock:
///            lockname: master
///            mongo-timeout: 1s
///            lock-ttl: 10s
/// ```
///
/// ## Static options:
/// name           | Description  | Default value
/// -------------- | ------------ | -------------
/// lockname       | name of the lock | --
/// lock-ttl       | TTL of the lock; must be at least as long as the duration between subsequent cancellation checks, otherwise brain split is possible | --
/// mongo-timeout     | timeout, must be at least 2*lock-ttl | --
/// restart-delay  | how much time to wait after failed task restart | 100ms

// clang-format on

class DistLockComponentBase : public components::LoggableComponentBase {
 public:
  DistLockComponentBase(const components::ComponentConfig&,
                        const components::ComponentContext&,
                        storages::mongo::Collection);

  ~DistLockComponentBase() override;

  dist_lock::DistLockedWorker& GetWorker();

 protected:
  /// Override this function with anything that must be done under the mongo
  /// lock.
  ///
  /// ## Example implementation
  ///
  /// ```cpp
  /// void MyDistLockComponent::DoWork()
  /// {
  ///     while (!engine::ShouldCancel())
  ///     {
  ///         // If Foo() or other function in DoWork() throws an exception,
  ///         // DoWork() will be restarted in `restart-delay` seconds.
  ///         Foo();
  ///
  ///         // Check for cancellation after cpu-intensive Foo().
  ///         // You must check for cancellation at least every `lock-ttl`
  ///         // seconds to have time to notice lock prolongation failure.
  ///         if (engine::ShouldCancel()) break;
  ///
  ///         Bar();
  ///     }
  /// }
  /// ```
  ///
  /// @note `DoWork` must honour task cancellation and stop ASAP when
  /// it is cancelled, otherwise brain split is possible (IOW, two different
  /// users do work assuming both of them hold the lock, which is not true).
  virtual void DoWork() = 0;

  /// Must be called in ctr
  void Start();

  /// Must be called in dtr
  void Stop();

 private:
  std::unique_ptr<dist_lock::DistLockedWorker> worker_;
  ::utils::statistics::Entry statistics_holder_;
};

}  // namespace storages::mongo