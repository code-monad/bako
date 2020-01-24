#ifndef __AOI_TASK_HH__
#define __AOI_TASK_HH__

#include <boost/asio/io_context.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/uuid/uuid.hpp>

#include <functional>
#include <initializer_list>
#include <string>

namespace aoi {
  class basic_task {
  public:
    basic_task(const std::string& name);
    virtual const std::string& name();
    virtual void process() = 0;
    virtual void set_done(const bool done = true);
    virtual void set_have_next(const bool have_next = true);
    virtual void set_next(std::shared_ptr<basic_task> next);
    virtual void set_executor(std::shared_ptr<boost::asio::io_context> executor);
    virtual boost::uuids::uuid uuid() noexcept { return _uuid; }
    virtual std::shared_ptr<basic_task> next() noexcept { return _next; }
    virtual std::shared_ptr<boost::asio::io_context> executor() noexcept { return _executor; }
    virtual const bool is_done() noexcept { return _done_flag; }
    virtual const bool have_next() noexcept { return _have_next; }
  private:
    bool _done_flag = false;
    bool _have_next = false;
    std::shared_ptr<basic_task> _next = nullptr;
    std::shared_ptr<boost::asio::io_context> _executor;
    boost::uuids::uuid _uuid;
    const std::string _name;

  };


  class term_task : public basic_task {
  public:
    typedef std::vector<std::string> command_t;
    typedef std::function<void(const std::string, int, const std::string)> handler_t;
    term_task(const std::string& name,const command_t& command, const handler_t& handler = std::move(handler_t{}) );
    term_task(const std::string& name,const std::initializer_list<std::string> command, const handler_t& handler = std::move(handler_t{}) );

    void process_detail(const std::string name, const aoi::term_task::command_t& command, boost::asio::yield_context yield_context);
    void process() override;



  private:

    void default_handler(const std::string& name, int ret, const std::string& buffer);

    const command_t _cmd;
    const handler_t _handler;
  };

}

#endif //__AOI_TASK_HH__
