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
#ifndef LIBBITCOIN_DATABASE_SLAB_MANAGER_HPP
#define LIBBITCOIN_DATABASE_SLAB_MANAGER_HPP

#include <cstddef>
#include <bitcoin/system.hpp>
#include <bitcoin/database/define.hpp>
#include <bitcoin/database/memory/memory.hpp>
#include <bitcoin/database/memory/storage.hpp>

namespace libbitcoin {
namespace database {

/// The slab manager represents a growing collection of various sized
/// slabs of data on disk. It will resize the file accordingly and keep
/// track of the current end pointer so new slabs can be allocated.
template <typename Link>
class slab_manager
  : system::noncopyable
{
public:
    // This cast is a VC++ workaround is OK because Link must be unsigned.
    //static constexpr Link empty = std::numeric_limits<Link>::max();
    static const Link not_allocated = (Link)bc::max_uint64;

    slab_manager(storage& file, size_t header_size);

    /// Create slab manager.
    bool create();

    /// Prepare manager for use.
    bool start();

    /// Commit total slabs size to the file.
    void commit();

    /// Get the size of all slabs and size prefix (excludes header).
    Link payload_size() const;

    /// Allocate a slab and return its position, commit after writing.
    Link allocate(size_t size);

    /// Check if link is past eof
    bool past_eof(Link link) const;

    /// Return memory object for the slab at the specified position.
    memory_ptr get(Link position) const;

private:
    // Read the size of the data from the file.
    void read_size();

    // Write the size of the data from the file.
    void write_size() const;

    // This class is thread and remap safe.
    storage& file_;
    const Link header_size_;

    // Payload size is protected by mutex.
    Link payload_size_;
    mutable system::shared_mutex mutex_;
};

} // namespace database
} // namespace libbitcoin

#include <bitcoin/database/impl/slab_manager.ipp>

#endif
