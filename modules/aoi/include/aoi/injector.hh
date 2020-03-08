#ifndef __AOI_INJECTOR_HH__
#define __AOI_INJECTOR_HH__

#include <boost/asio/io_context.hpp>
#include <nlohmann/json.hpp>

#include <memory>


namespace aoi {
  class basic_task;
  class work_group;

  class injector {
  public:
    typedef nlohmann::json input_type;
    
    injector(std::shared_ptr<boost::asio::io_context> executor);
    std::unique_ptr<work_group> parse(const input_type& input);
    std::shared_ptr<basic_task> parse_task(const input_type& input, const std::string task_type = "term", const std::string name = "anoymouns-task");
  private:
    std::shared_ptr<boost::asio::io_context> _executor;
  };
} // namespace aoi

#endif // __AOI_INJECTOR_HH__
