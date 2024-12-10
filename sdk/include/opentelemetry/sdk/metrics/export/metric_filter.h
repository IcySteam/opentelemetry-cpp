// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/nostd/string_view.h"
#include "opentelemetry/sdk/instrumentationscope/instrumentation_scope.h"
#include "opentelemetry/sdk/metrics/data/metric_data.h"
#include "opentelemetry/sdk/metrics/instruments.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

/**
 * MetricFilter defines the interface which enables the MetricReader’s
 * registered MetricProducers or the SDK’s MetricProducer to filter aggregated
 * data points (Metric Points) inside its Produce operation. The filtering is
 * done at the MetricProducer for performance reasons.
 *
 * The MetricFilter allows filtering an entire metric stream - dropping or
 * allowing all its attribute sets - by its TestMetric operation, which accepts
 * the metric stream information (scope, name, kind and unit) and returns an
 * enumeration: kAccept, kDrop or kAcceptPartial. If the latter returned, the
 * TestAttributes operation is to be called per attribute set of that metric
 * stream, returning an enumeration determining if the data point for that
 * (metric stream, attributes) pair is to be allowed in the result of the
 * MetricProducer Produce operation.
 */
class MetricFilter
{
public:
  enum class MetricFilterResult
  {
    kAccept,
    kDrop,
    kAcceptPartial,
  };

  enum class AttributesFilterResult
  {
    kAccept,
    kDrop,
  };

  MetricFilter()          = default;
  virtual ~MetricFilter() = default;

  /**
   * TestMetric is called once for every metric stream, in each MetricProducer
   * Produce operation.
   */
  virtual MetricFilterResult TestMetric(
      const opentelemetry::sdk::instrumentationscope::InstrumentationScope &scope,
      opentelemetry::nostd::string_view name,
      const InstrumentType &type,
      opentelemetry::nostd::string_view unit) noexcept = 0;

  /**
   * TestAttributes determines for a given metric stream and attribute set if
   * it should be allowed or filtered out.
   *
   * This operation should only be called if TestMetric operation returned
   * kAcceptPartial for the given metric stream arguments.
   */
  virtual AttributesFilterResult TestAttributes(
      const opentelemetry::sdk::instrumentationscope::InstrumentationScope &scope,
      opentelemetry::nostd::string_view name,
      const InstrumentType &type,
      opentelemetry::nostd::string_view unit,
      const PointAttributes &attributes) noexcept = 0;
};

}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
