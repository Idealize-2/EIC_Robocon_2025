#include <functional>

class GlobalDelay {
  private:
    function<void()> storedFunction;
    long time;
    bool isRunning;
  public:
    GlobalDelay(std::function<void()> func)
    {
      this->storedFunction = func;
    }
    void call(long time)
    {
      if( this->isRunning )
      {
        func();
        is runnung
      }
      else
      {
        this->time = time;
        this->isRunning = true;
      }
    }
};