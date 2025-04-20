#include <functional>

class GlobalDelay {
  private:
    //storedFunction = []() { runMotor(200); };
    std::function<void(  )> storedFunction;
    std::function<void(  )> endFunction;
    
    unsigned long long time = 0, start_time = 0;  
    bool isActivated = true;
  public:

    GlobalDelay(std::function<void(  )> func , std::function<void(  )> endFunc , long long time)
    {
      this->storedFunction = func;
      this->endFunction = endFunc;
      this->time = time;
      this->start_time = millis();
      //this->time = time;
    }

    // Destructor
    ~GlobalDelay() {
      endFunction();
      // Optional debug message
      //Serial.println("GlobalDelay destroyed");
    }

    void call()
    {
      if( !this->alive() )return;
      // Serial.print( loop );
      // Serial.print( ' ' );
      storedFunction();
      //delay(1);
      //Serial.println(millis() - start_time);
      if( millis() - start_time >= time  )
      {
        start_time = 0;
        this->isActivated = false;
      }     

    }

    bool alive()
    {
      return this->isActivated;
    }
    // void activate(long time){
    //   this->isActivated = true;
    //   this->time = time;
    //   this->loop = 0;
    // }
    // void deactivate(){
    //   this->isActivated = false;
    //   this->loop = 0;
    // }

};