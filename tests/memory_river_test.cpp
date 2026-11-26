#include "MemoryRiver.hpp"

#include <array>
#include <cassert>
#include <cstdio>
#include <random>
#include <vector>

int main() {
    using Record = std::array<int, 16>;
    const std::string path = "memory_river_test.bin";
    MemoryRiver<Record, 5> river;
    river.initialise(path);
    for (int n = 1; n <= 5; ++n) {
        int value = -1;
        river.get_info(value, n);
        assert(value == 0);
        river.write_info(n * -37, n);
    }

    std::mt19937 rng(1336);
    std::vector<Record> expected;
    std::vector<int> indices;
    for (int i = 0; i < 2000; ++i) {
        Record value{};
        for (int &part : value) part = static_cast<int>(rng() % 1000000);
        const int index = river.write(value);
        assert(index == static_cast<int>(5 * sizeof(int) + i * sizeof(Record)));
        expected.push_back(value);
        indices.push_back(index);
        const unsigned chosen = rng() % expected.size();
        river.update(value, indices[chosen]);
        expected[chosen] = value;
        Record actual{};
        river.read(actual, indices[chosen]);
        assert(actual == expected[chosen]);
    }

    MemoryRiver<Record, 5> reopened(path);
    for (unsigned i = 0; i < expected.size(); ++i) {
        Record actual{};
        reopened.read(actual, indices[i]);
        assert(actual == expected[i]);
    }
    for (int n = 1; n <= 5; ++n) {
        int value = 0;
        reopened.get_info(value, n);
        assert(value == n * -37);
    }
    river.Delete(indices.front());
    Record value{};
    assert(river.write(value) > indices.back());
    river.initialise();
    assert(river.write(value) == 5 * sizeof(int));
    std::remove(path.c_str());
}
