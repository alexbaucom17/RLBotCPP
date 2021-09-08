#pragma once

#include "rlbot/interface.h"

#include <cstdlib>

namespace rlbot {
template <typename type> class FlatbufferContainer {
private:
  char *data;
  size_t size;
  const type *flatbuffer;

public:
  FlatbufferContainer(ByteBuffer buffer) {
    size = buffer.size;
    data = (char *)malloc(size);
    memcpy(data, buffer.ptr, size);

    flatbuffer = flatbuffers::GetRoot<type>(data);
  }

  ~FlatbufferContainer() { free(data); }

  FlatbufferContainer(const FlatbufferContainer &flatbuffercontainer) {
    size = flatbuffercontainer.size;
    data = (char *)malloc(size);
    memcpy(data, flatbuffercontainer.data, size);

    flatbuffer = flatbuffers::GetRoot<type>(data);
  }

  FlatbufferContainer(FlatbufferContainer &&flatbuffercontainer) {
    size = flatbuffercontainer.size;
    data = flatbuffercontainer.data;

    flatbuffer = flatbuffers::GetRoot<type>(data);

    flatbuffercontainer.data = nullptr;
    flatbuffercontainer.size = 0;
  }

  FlatbufferContainer<type>
  operator=(const FlatbufferContainer &flatbuffercontainer) {
    size = flatbuffercontainer.size;
    data = (char *)malloc(size);
    memcpy(data, flatbuffercontainer.data, size);

    flatbuffer = flatbuffers::GetRoot<type>(data);
    return *this;
  }

  FlatbufferContainer<type>
  operator=(FlatbufferContainer &&flatbuffercontainer) {
    size = flatbuffercontainer.size;
    data = flatbuffercontainer.data;

    flatbuffer = flatbuffers::GetRoot<type>(data);

    flatbuffercontainer.data = nullptr;
    flatbuffercontainer.size = 0;
    return *this;
  }

  const type *getRoot() const { return flatbuffer; }
  const type *operator->() const { return flatbuffer; }
  std::vector<uint8_t> getBufferCopy() const {
    std::vector<uint8_t> vec;
    vec.reserve(size);
    for (int i = 0; i < size; i++) {
      uint8_t d = static_cast<uint8_t>(*(data+i));
      vec.push_back(d);
    }
    return vec;
  }
};
} // namespace rlbot
