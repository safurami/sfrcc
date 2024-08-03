#ifndef MY_H
#define MY_H

// How many times does the vector increases when resize().
#define VECTOR_INCREASE 2

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

template <typename T>
class vector
{
  T *data_pointer;
  int capacity;
  int size;
public:
  vector(int init_size)
  {
    this->capacity = init_size;
    this->data_pointer = new T[init_size]();
    this->size = 0;
  }
  ~vector()
  {
    delete[] this->data_pointer;
  }
  void resize()
  {
    T *old_pointer = this->data_pointer;
    this->capacity *= VECTOR_INCREASE;
    this->data_pointer = new T[this->capacity]();
    for(int i = 0; i < this->size; i++)
    {
      this->data_pointer[i] = old_pointer[i];
    }
    delete[] old_pointer;
  }
  void push_back(T el)
  {
    if(this->size == this->capacity)
    {
      this->resize();
    }
    this->data_pointer[this->size++] = el;
  }
  // TODO more methods.

};

}

#endif /* MY_H */
