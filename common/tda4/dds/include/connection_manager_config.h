#ifndef _CONNECTION_MANAGER_CONFIG_H_
#define _CONNECTION_MANAGER_CONFIG_H_
#include <string>
#include <vector>

namespace module::connection {
struct ConnectionManagerConfig {
  std::vector<std::string> publish_topics;
  std::vector<std::string> subscribe_topics;
  // 其他参数，比如我们的dds需要contex.json，这些都方在这里
  std::vector<std::string> other_params;
};
}  // namespace module::connection
#endif