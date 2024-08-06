#ifndef MY_H
#define MY_H

// How many times does the vector increases when resize().
#define VECTOR_INCREASE 2

namespace my
{

bool isdigit(char c);
bool isalpha(char c);
bool isalnum(char c);
bool strcmp(const char *src, const char *dst);
bool strcmpn(const char *src, const char *dst, int count);
int strlen(const char *string);

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

template <typename T>
class vector
{
  T *data_pointer;
  int capacity;
  int vec_size;
public:
  vector(int init_size)
  {
    this->capacity = init_size;
    this->data_pointer = new T[init_size]();
    this->vec_size = 0;
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
    for(int i = 0; i < this->vec_size; i++)
    {
      this->data_pointer[i] = old_pointer[i];
    }
    delete[] old_pointer;
  }
  void push_back(T el)
  {
    if(this->vec_size == this->capacity)
    {
      this->resize();
    }
    this->data_pointer[this->vec_size++] = el;
  }
  int size()
  {
    return this->vec_size;
  }
  void clear()
  {
    // clear() deletes previous data, setting size to 0,
    // and allocating new memory for this->capacity.
    delete[] this->data_pointer;
    this->vec_size = 0;
    this->data_pointer = new T[this->capacity]();
  }
  bool empty()
  {
    return !this->vec_size;
  }
  T *data()
  {
    return this->data_pointer;
  }
  T &at(int index)
  {
    if(index >= this->vec_size || index < 0)
    {
      return *this->data_pointer;
    }
    return this->data_pointer[index];
  }
  T &back()
  {
    return this->data_pointer[this->vec_size - 1];
  }
  T &front()
  {
    return *this->data_pointer;
  }
  void pop_back()
  {
    this->vec_size--;
  }
  void swap(my::vector<T> &other)
  {
    T *save = this->data_pointer;
    int s_capacity = this->capacity;
    int s_size = this->vec_size;
    this->data_pointer = other.data_pointer;
    this->vec_size = other.vec_size;
    this->capacity = other.capacity;
    other.data_pointer = save;
    other.capacity = s_capacity;
    other.vec_size = s_size;
  }
  int max_size()
  {
    return this->capacity;
  }
};

}

#endif /* MY_H */
