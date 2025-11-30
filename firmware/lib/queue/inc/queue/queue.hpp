#ifndef LIB_QUEUE_QUEUE_HPP
#define LIB_QUEUE_QUEUE_HPP

#include "pico/stdlib.h"
#include "pico/util/queue.h"

namespace lib::queue {

enum class CoreCommand : uint32_t {
    none,
    send_key,
};

class Queue
{
public:
    Queue() {}
    void init();
    [[nodiscard]] bool add(CoreCommand command);
    [[nodiscard]] bool get(CoreCommand &command);

private:
    queue_t _queue;
};

extern Queue g_queue_to_core0;
extern Queue g_queue_to_core1;

}  // namespace lib::queue

#endif  // LIB_QUEUE_QUEUE_HPP
