#ifndef LIB_QUEUE_QUEUE_HPP
#define LIB_QUEUE_QUEUE_HPP

#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include <array>

namespace lib::queue {

enum class CoreCommandType : uint8_t {
    none,
    send_key,
    square_number,
    div_by_2_number,
};

struct CoreCommand {
    CoreCommandType command_type;
    std::array<uint8_t, 64> payload;
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
