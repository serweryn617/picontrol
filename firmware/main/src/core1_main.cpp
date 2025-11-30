#include "main/core1_main.hpp"
#include "queue/queue.hpp"

using namespace lib::queue;

void core1_main() {
  while (true) {
    CoreCommand cmd{};
    bool getOk = g_queue_to_core1.get(cmd);
    if (!getOk) {
      continue;
    }

    uint32_t number{};
    switch (cmd.command_type) {

      case CoreCommandType::square_number:
        number = cmd.payload[0];
        break;
      case CoreCommandType::div_by_2_number:
        break;
    }
  }
}
