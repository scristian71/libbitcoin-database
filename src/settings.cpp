/**
 * Copyright (c) 2011-2019 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/database/settings.hpp>

#include <boost/filesystem.hpp>

namespace libbitcoin {
namespace database {

using namespace boost::filesystem;
using namespace bc::system;

settings::settings()
  : directory("blockchain"),

    flush_writes(false),
    cache_capacity(0),
    file_growth_rate(5),

    // Hash table sizes (must be configured).
    block_table_buckets(0),
    transaction_table_buckets(0),
    payment_table_buckets(0),

    // Minimum file sizes.
    block_table_size(1),
    candidate_index_size(1),
    confirmed_index_size(1),
    transaction_index_size(1),
    transaction_table_size(1),
    payment_index_size(1),
    payment_table_size(1),

    // Neutrino filter database
    neutrino_filter_table_buckets(0),
    neutrino_filter_table_size(1)
{
}

settings::settings(config::settings context)
  : settings()
{
    switch (context)
    {
        case config::settings::mainnet:
        {
            block_table_buckets = 650000;
            transaction_table_buckets = 110000000;
            payment_table_buckets = 107000000;
            block_table_size = 80000000;
            candidate_index_size = 3000000;
            confirmed_index_size = 3000000;
            transaction_index_size = 3000000000;
            transaction_table_size = 220000000000;
            payment_index_size = 100000000000;
            payment_table_size = 100000000;
            neutrino_filter_table_buckets = 650000;
            neutrino_filter_table_size = 80000000;
            break;
        }

        case config::settings::testnet:
        {
            // TODO: optimize for testnet.
            block_table_buckets = 650000;
            transaction_table_buckets = 110000000;
            payment_table_buckets = 107000000;
            block_table_size = 42;
            candidate_index_size = 42;
            confirmed_index_size = 42;
            transaction_index_size = 42;
            transaction_table_size = 42;
            payment_index_size = 42;
            payment_table_size = 42;
            neutrino_filter_table_buckets = 650000;
            neutrino_filter_table_size = 42;
            break;
        }

        case config::settings::regtest:
        {
            // TODO: optimize for regtest.
            block_table_buckets = 650000;
            transaction_table_buckets = 110000000;
            payment_table_buckets = 107000000;
            block_table_size = 42;
            candidate_index_size = 42;
            confirmed_index_size = 42;
            transaction_index_size = 42;
            transaction_table_size = 42;
            payment_index_size = 42;
            payment_table_size = 42;
            neutrino_filter_table_buckets = 650000;
            neutrino_filter_table_size = 42;
            break;
        }

        default:
        case config::settings::none:
        {
        }
    }
}

} // namespace database
} // namespace libbitcoin
