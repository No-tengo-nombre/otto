#pragma once

#include <cstddef>
#include <otto/otto.h>
#include <stdexcept>
#include <vector>

namespace otto {

class Buffer {
public:
  Buffer(const size_t data_size);
  Buffer(const void *data, const size_t len, const size_t data_size);
  template <typename T> Buffer();
  template <typename T> Buffer(const std::vector<T> &data);

  ~Buffer();

  static Buffer                       &zero(const size_t len, const size_t data_size);
  static Buffer                       &with_capacity(const size_t capacity, const size_t data_size);
  template <typename T> static Buffer &zero(const size_t len);
  template <typename T> static Buffer &with_capacity(const size_t capacity);

  template <typename T> T    get(const size_t i) const;
  void                       get(const size_t i, void *out) const;
  template <typename T> void set(const size_t i, T val);
  template <typename T> void set(const size_t i, T *val);
  template <typename T> void push(T val);
  template <typename T> void push(T *val);
  void                       resize(const size_t new_capacity);
  void                       extend(const void *vals, const size_t len);
  template <typename T> void extend(const std::vector<T> &vals);

private:
  otto_buffer_t h;
};

/* Template members definitions */

template <typename T> Buffer::Buffer() {
  size_t data_size = sizeof(T);
  if (otto_buffer_new(data_size, &h).status != OTTO_SUCCESS) {
    throw std::runtime_error("Failed to create buffer");
  }
}

template <typename T> Buffer::Buffer(const std::vector<T> &data) {
  size_t data_size = sizeof(T);
  if (otto_buffer_from_array(data.data, data.len, data_size, &h).status != OTTO_SUCCESS) {
    throw std::runtime_error("Failed to create buffer");
  }
}

template <typename T> Buffer &Buffer::zero(const size_t len) {
  size_t data_size = sizeof(T);
  return Buffer::zero(len, data_size);
}

template <typename T> Buffer &Buffer::with_capacity(const size_t capacity) {
  size_t data_size = sizeof(T);
  return Buffer::with_capacity(capacity, data_size);
}

template <typename T> T Buffer::get(const size_t i) const {
  T out;
  if (otto_buffer_get(&h, i, &out).status != OTTO_SUCCESS) {
    throw std::runtime_error("Failed to get element");
  }
  return out;
}

} // namespace otto
