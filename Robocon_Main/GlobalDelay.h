#include <functional>

class GlobalDelay {
  private:
    function<void(long time)> storedFunction;
    long time, loop;    
    bool isActivated = false;
  public:
    GlobalDelay(std::function<void()> func)
    {
      this->storedFunction = func;
    }
    void call()
    {
      if( this->isActivated )
      {
        storedFunction();
        loop++;
        if( loop >= time )
        {
          loop = 0;
          this->Activated = false;
        }  
      }    

    }
    void activate(long time){
      this->Activated = true;
      this->time = time;
      this->loop = 0;
    }
    void deactivate(){
      this->Activated = false;
      this->loop = 0;
    }
};