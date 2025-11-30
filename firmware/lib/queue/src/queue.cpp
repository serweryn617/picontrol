#include "queue/queue.hpp"

namespace lib::queue {

void Queue::init()
{
    queue_init(&_queue, sizeof(CoreCommand), 64);
}

[[nodiscard]] bool Queue::add(CoreCommand command)
{
    return queue_try_add(&_queue, &command);
}

[[nodiscard]] bool Queue::get(CoreCommand &command)
{
    return queue_try_remove(&_queue, &command);
}

Queue g_queue_to_core0;
Queue g_queue_to_core1;

}  // namespace lib::queue
