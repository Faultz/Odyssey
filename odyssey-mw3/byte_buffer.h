// Copyright 2022 xensik. All rights reserved.
//
// Use of this source code is governed by a GNU GPLv3 license
// that can be found in the LICENSE file.

#pragma once

template <typename T>
T reverse(T u)
{
    return u;
    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}


class byte_buffer
{
public:
    using ptr = byte_buffer*;

private:
    std::vector<std::uint8_t> data_;
    std::uint32_t size_;
    std::uint32_t pos_;

public:
    byte_buffer();
    byte_buffer(std::uint32_t size);
    byte_buffer(const std::vector<std::uint8_t>& data);
    ~byte_buffer();

    template <typename T>
    auto read() -> T
    {
        if (pos_ + sizeof(T) > size_) return T {};

        auto ret = *reinterpret_cast<T*>(data_.data() + pos_);
        pos_ += sizeof(T);
        return ret;
    }

    template <typename T>
    void write(T data)
    {
        if (pos_ + sizeof(T) > size_) return;

        T* mem = reinterpret_cast<T*>(data_.data() + pos_);
        std::memcpy(mem, &data, sizeof(T));
        pos_ += sizeof(T);
    }

    template <typename T>
    auto read_endian() -> T
    {
        if (pos_ + sizeof(T) > size_) return T {};

        std::array<std::uint8_t, sizeof(T)> mem;

        for (auto i = 0; i < sizeof(T); i++)
        {
            mem[i] = reinterpret_cast<std::uint8_t*>(data_.data() + pos_)[sizeof(T) - 1 - i];
        }

        pos_ += sizeof(T);

        return *reinterpret_cast<T*>(mem.data());
    }

    template <typename T>
    void write_endian(T data)
    {
        if (pos_ + sizeof(T) > size_) return;

        auto* mem = data_.data() + pos_;

        for (auto i = 0; i < sizeof(T); i++)
        {
            mem[i] = reinterpret_cast<std::uint8_t*>(&data)[sizeof(T) - 1 - i];
        }

        pos_ += sizeof(T);
    }

    void clear();
    auto is_avail() -> bool;
    void seek(std::uint32_t count);
    void seek_neg(std::uint32_t count);
    void write_string(const std::string& data);
    void write_c_string(const std::string& data);
    auto read_c_string()->std::string;
    auto print_bytes(std::uint32_t pos, std::uint32_t count)->std::string;
    auto pos()->std::uint32_t;
    void pos(std::uint32_t pos);
    auto align(std::uint32_t size)->std::uint32_t;
    auto buffer()->std::vector<std::uint8_t>&;
};

