#include <functional>

class GlobalDelay {
  private:
    function<void( float )> storedFunction1 = nullptr;
    function<void(  )> storedFunction2 = nullptr;
    long time = 0, loop = 0; 
    float param1 = 0;   
    bool isActivated = false;
  public:
    GlobalDelay(function<void( float )> func)
    {
      this->storedFunction1 = func;
    }
    GlobalDelay(function<void(  )> func)
    {
      this->storedFunction2 = func;
    }

    // Destructor
    ~GlobalDelay() {
      // Optional debug message
      Serial.println("GlobalDelay destroyed");
    }

    void call()
    {
      if( !this->isActivated )return;

      if (storedFunction1) 
        storedFunction1();
      else if (storedFunction2) 
        storedFunction2();

      loop++;
      if( loop >= time )
      {
        loop = 0;
        this->isActivated = false;
      }     

    }
    void activate(long time){
      this->isActivated = true;
      this->time = time;
      this->loop = 0;
    }
    void activate(long time , float param){
      this->isActivated = true;
      this->time = time;
      this->loop = 0;
      this->param1 = param;
    }
    void deactivate(){
      this->isActivated = false;
      this->loop = 0;
    }
};