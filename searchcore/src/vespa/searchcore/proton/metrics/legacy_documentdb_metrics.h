// Copyright 2016 Yahoo Inc. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.

#pragma once

#include "legacy_attribute_metrics.h"
#include "executor_metrics.h"
#include <vespa/metrics/metrics.h>
#include "sessionmanager_metrics.h"
#include "feed_metrics.h"
#include <vespa/searchcore/proton/matching/matching_stats.h>

namespace proton {

/**
 * Metric set for all legacy metrics reported for a document db.
 *
 * All these metrics have the document type name as part of the metric name,
 * which is not flexible for setting up default metric graph dashboards.
 *
 * @deprecated Use DocumentDBTaggedMetrics for all new metrics.
 */
struct LegacyDocumentDBMetrics : metrics::MetricSet
{
    struct IndexMetrics : metrics::MetricSet {
        metrics::LongValueMetric memoryUsage;
        metrics::LongValueMetric docsInMemory;
        metrics::LongValueMetric diskUsage;

        IndexMetrics(metrics::MetricSet *parent);
    };

    struct DocstoreMetrics : metrics::MetricSet {
        metrics::LongValueMetric memoryUsage;
        metrics::LongCountMetric cacheLookups;
        size_t hits;
        metrics::LongAverageMetric cacheHitRate;
        metrics::LongValueMetric cacheElements;
        metrics::LongValueMetric cacheMemoryUsed;

        DocstoreMetrics(metrics::MetricSet *parent);
    };

    struct MatchingMetrics : metrics::MetricSet {
        metrics::LongCountMetric docsMatched;
        metrics::LongCountMetric docsRanked;
        metrics::LongCountMetric docsReRanked;
        metrics::LongCountMetric queries;
        metrics::DoubleAverageMetric queryCollateralTime;
        metrics::DoubleAverageMetric queryLatency;

        struct RankProfileMetrics : metrics::MetricSet {
            struct DocIdPartition : metrics::MetricSet {
                metrics::LongCountMetric docsMatched;
                metrics::LongCountMetric docsRanked;
                metrics::LongCountMetric docsReRanked;
                metrics::DoubleAverageMetric active_time;
                metrics::DoubleAverageMetric wait_time;

                typedef vespalib::LinkedPtr<DocIdPartition> LP;
                DocIdPartition(const std::string &name, metrics::MetricSet *parent);
                void update(const matching::MatchingStats::Partition &stats);
            };
            typedef std::vector<DocIdPartition::LP> DocIdPartitions;

            typedef vespalib::LinkedPtr<RankProfileMetrics> LP;

            metrics::LongCountMetric     queries;
            metrics::LongCountMetric     limited_queries;        
            metrics::DoubleAverageMetric matchTime;
            metrics::DoubleAverageMetric groupingTime;
            metrics::DoubleAverageMetric rerankTime;
            DocIdPartitions              partitions;

            RankProfileMetrics(const std::string &name,
                               size_t numDocIdPartitions,
                               metrics::MetricSet *parent);
            void update(const matching::MatchingStats &stats);

        };
        typedef std::map<std::string, RankProfileMetrics::LP> RankProfileMap;
        RankProfileMap rank_profiles;

        void update(const matching::MatchingStats &stats);
        MatchingMetrics(metrics::MetricSet *parent);
    };

    struct SubDBMetrics : metrics::MetricSet
    {
        struct DocumentMetaStoreMetrics : metrics::MetricSet
        {
            metrics::LongValueMetric lidLimit;
            metrics::LongValueMetric usedLids;
            metrics::LongValueMetric lowestFreeLid;
            metrics::LongValueMetric highestUsedLid;
            metrics::DoubleValueMetric lidBloatFactor;
            metrics::DoubleValueMetric lidFragmentationFactor;

            DocumentMetaStoreMetrics(metrics::MetricSet *parent);
        };

        LegacyAttributeMetrics attributes;
        DocumentMetaStoreMetrics docMetaStore;
        SubDBMetrics(const vespalib::string &name, metrics::MetricSet *parent);
    };

    IndexMetrics                                 index;
    LegacyAttributeMetrics                       attributes;
    DocstoreMetrics                              docstore;
    MatchingMetrics                              matching;
    ExecutorMetrics                              executor;
    ExecutorMetrics                              indexExecutor;
    PerDocTypeFeedMetrics                        feed;
    search::grouping::SessionManagerMetrics      sessionManager;
    SubDBMetrics                                 ready;
    SubDBMetrics                                 notReady;
    SubDBMetrics                                 removed;
    metrics::SumMetric<metrics::LongValueMetric> memoryUsage;
    metrics::LongValueMetric                     numDocs;
    metrics::LongValueMetric                     numActiveDocs;
    metrics::LongValueMetric                     numIndexedDocs;
    metrics::LongValueMetric                     numStoredDocs;
    metrics::LongValueMetric                     numRemovedDocs;
    metrics::LongValueMetric                     numBadConfigs;
    size_t                                      _maxNumThreads;

    LegacyDocumentDBMetrics(const std::string &docTypeName, size_t maxNumThreads);
};

} // namespace proton

