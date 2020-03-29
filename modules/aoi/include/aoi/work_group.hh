#ifndef __AOI_WORKGROUP_HH__
#define __AOI_WORKGROUP_HH__

#include <boost/uuid/uuid.hpp>
#include <boost/asio/io_context.hpp>

#include <map>
#include <memory>

namespace aoi {
  class basic_task;

  // chained tasks, like a linked list
  class work_group {
  public:
    work_group(std::shared_ptr<boost::asio::io_context> executor);
    void add_task(std::shared_ptr<basic_task> task);

    std::shared_ptr<basic_task> begin() noexcept;
    std::shared_ptr<basic_task> last() noexcept;
    std::shared_ptr<basic_task> end() noexcept { return std::shared_ptr<basic_task>(nullptr); }

    void execute();

  private:
    std::shared_ptr<boost::asio::io_context> _executor;
    std::shared_ptr<basic_task> _begin = nullptr;
    std::shared_ptr<basic_task> _last = nullptr;
    std::map<boost::uuids::uuid, std::shared_ptr<basic_task>> _tasks;
  };
}

#endif // __AOI_WORKGROUP_HH__
