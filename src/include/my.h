#ifndef MY_H
#define MY_H

namespace my
{

// Basic implementation of unique pointer.
template <typename T>
class unique_ptr
{
  T *pointer;
public:
  unique_ptr(const unique_ptr &other) = delete;
  unique_ptr& operator =(unique_ptr &other) = delete;

  unique_ptr(T *ptr)
  {
    this->pointer = ptr;
  }
  ~unique_ptr()
  {
    delete this->pointer;
  }
  T& operator*()
  {
    return *this->pointer;
  }
  T* operator->()
  {
    return this->pointer;
  }
  T* get()
  {
    return this->pointer;
  }
};

bool isdigit(char c);

}

#endif /* MY_H */
