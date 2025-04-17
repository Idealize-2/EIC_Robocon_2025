#include <functional>

class GlobalDelay {
  private:
    //storedFunction = []() { runMotor(200); };
    function<void(  )> storedFunction;
    long time = 0, loop = 0;  
    bool isActivated = false;
  public:
    GlobalDelay(function<void(  )> func)
    {
      this->storedFunction = func;
    }

    // Destructor
    ~GlobalDelay() {
      // Optional debug message
      Serial.println("GlobalDelay destroyed");
    }

    void call()
    {
      if( !this->isActivated )return;
      storedFunction();
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
    void activate(long time){
      this->isActivated = true;
      this->time = time;
      this->loop = 0;
    }
    void deactivate(){
      this->isActivated = false;
      this->loop = 0;
    }
};