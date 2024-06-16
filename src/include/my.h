#ifndef H_MY
#define H_MY

#include <chrono>

#define my_assert(expr) (my::assert(expr, __FILE__, __LINE__, __func__)) // To use assert, use this macro

namespace my
{

/*
 * My poor implementation of vector. I didnt even write pop_back() and erase() methods XD.
 */
template <typename T>
class vector
{
private:
  T* m_array;
  int m_capacity;
  int m_size;
public:
  vector(vector &) = delete;
  vector(int capacity)
  {
    this->m_array = new T[capacity];
    this->m_capacity = capacity;
    this->m_size = 0;
  }
  ~vector()
  {
    delete[] this->m_array;
  }
  T& front()
  {
    return m_array[0];
  }
  T& back()
  {
    return m_array[m_size - 1];
  }
  T& get(int index)
  {
    if(index > this->m_size - 1 || index < 0)
    {
      return this->m_array[0];
    }
    return this->m_array[index];
  }
  T* get_raw_pointer(int index)
  {
    if(index > this->m_size - 1 || index < 0)
    {
      return this->m_array;
    }
    return this->m_array + index;
  }
  void push_back(T &val)
  {
    if(this->m_size == this->m_capacity)
    {
      this->resize();
    }
    this->m_array[m_size] = val;
    this->m_size++;
  }
  void resize()
  {
    this->m_capacity *= 2;
    T* tmp = this->m_array;
    this->m_array = new T[m_capacity];
    for(int i = 0; i < this->m_size; i++)
    {
      this->m_array[i] = tmp[i];
    }
    delete[] tmp;
  }
  int get_size()
  {
    return this->m_size;
  }
  int get_capacity()
  {
    return this->m_capacity;
  }
};

class timer
{
private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start_point;
public:
  timer();
  ~timer();
};

template <typename T>
T&& move(T& val)
{
  return (T&&)val;
}

bool isdigit(char);
bool isalnum(char);
int strlen(const char *);
int strncpy(char *, const char *, int);
bool isalpha(char);
int strcmp(const char *, const char *);
void assert(const bool, const char *, const int, const char *);
int atoi(const char *);


}

#endif
