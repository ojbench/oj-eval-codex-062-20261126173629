# MemoryRiver Easy

`MemoryRiver.hpp` implements the required template. The file starts with
`info_len` zero-initialized integers. Each `write` appends the raw bytes of a
record and returns its byte offset. Reads and updates seek directly to that
offset; header operations use one-based integer positions. Deletion is a no-op
as permitted by the assignment. Each operation closes its stream so data is
available immediately to other instances and survives object destruction.

Record operations transfer O(sizeof(T)) bytes with O(1) auxiliary memory;
header operations take O(1), and initialization takes O(info_len).
Records must support the binary representation storage required by the problem;
objects owning pointers or dynamic allocations are not serializable this way.

## Local validation

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Werror -pedantic -I. tests/memory_river_test.cpp -o /tmp/problem062_test
(cd /tmp && ./problem062_test)
```

The test checks header initialization, randomized appends and updates, stable
offsets, reopening, header isolation, deletion without reuse, and truncation.

## Submission

The live problem accepts `cpp` source and supplies the template test harness.
Submit `MemoryRiver.hpp` through the client after committing and pushing it.
The generic executable build instructions in README do not describe this
header-only interface. The client's missing `submit_code` method has been
implemented using its existing API request and submission logging helpers.
