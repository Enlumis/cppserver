#ifndef		_ICALL_HH_
#define		_ICALL_HH_

class		ICall
{
public:
  virtual         ~ICall() {}
  virtual void    update() = 0;
};

#endif		/* _ICALL_HH_ */
