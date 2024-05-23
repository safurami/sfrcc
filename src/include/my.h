#ifndef H_MY
#define H_MY

namespace my
{

/*
 * My poor implementation of vector. I didnt even write pop_back() and erase() methods XD.
 */
template <typename T>
class vector
{
  T* array;
  int capacity;
  int size;
public:
  vector(vector &) = delete;
  vector(int capacity)
  {
    this->array = new T[capacity];
    this->capacity = capacity;
    this->size = 0;
  }
  ~vector()
  {
    delete[] this->array;
  }
  T& get(int index)
  {
    if(index > this->size - 1 || index < 0)
    {
      return this->array[0];
    }
    return this->array[index];
  }
  void push_back(T val)
  {
    if(this->size == this->capacity)
    {
      this->resize();
    }
    this->array[size] = val;
    this->size++;
  }
  void resize()
  {
    this->capacity *= 2;
    T* tmp = this->array;
    this->array = new T[capacity];
    for(int i = 0; i < this->size; i++)
    {
      this->array[i] = tmp[i];
    }
    delete[] tmp;
  }
  int get_size()
  {
    return this->size;
  }
  int get_capacity()
  {
    return this->capacity;
  }
};

bool isdigit(char);
bool isalnum(char);
int strlen(const char *);
int strncpy(char *, const char *, int);
bool isalpha(char);
int strcmp(const char *, const char *);

}

#endif
