#ifndef __AOI_LOG_HH__
#define __AOI_LOG_HH__

#include <spdlog/spdlog.h>

namespace aoi {
  class logger {

  public:
    logger(spdlog::level::level_enum level = spdlog::level::debug, const std::string& base_dir = "./log");
    std::shared_ptr<spdlog::logger> get();
    void set_level(spdlog::level::level_enum level);

  private:
    std::shared_ptr<spdlog::logger> _logger;
  };

  class file_logger {
  public:
    file_logger(const std::string& file_name, spdlog::level::level_enum level = spdlog::level::debug, const std::string base_dir = "./log/tasks");
    std::shared_ptr<spdlog::logger> get();
    void set_level(spdlog::level::level_enum level);

  private:
    std::shared_ptr<spdlog::logger> _logger;
  };
}

#endif //__AOI_LOG_HH__
