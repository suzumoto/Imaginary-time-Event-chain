#ifndef CLASS_BEAD_
#define CLASS_BEAD_

class bead
{
private:
  double position_;
  double dist_; //worldline[j].global_position() - worldline[j-1].global_position()
public:
  bead(double p): position_(p), dist_(0){};
  bead(): position_(0), dist_(0){};
  ~bead(){}
  void set_position(double p){position_ = p;}
  void add_position(double s){position_ += s;}
  void add_dist(double d){dist_ += d;}
  void set_dist(double d){dist_ = d;}
  void set_period(){
    while(position_ > L){
      position_ -= L;
    }
    while(position_ < 0){
      position_ += L;
    }
  }
  double position(){return position_;}
  double dist(){return dist_;}
};

#endif
