#ifndef H_SMTBL
#define H_SMTBL

class symbol_table
{
  class node
  {
  public:
    char *name;
    int size;
    int line_of_declaration;
    int address;
    node* next; // TODO ??

    node();
    ~node();
    node(node &);
    const char *get_name();
    void set_name(const char *name);
    void set_size(int);
    int get_size();
    void set_line(int);
    int get_line();
    void set_address(int);
    int get_address();
  };
  node* table; // Array for nodes
public:
  symbol_table();
  ~symbol_table();
  int install_id(const char *, const char *);
  static unsigned int hash(const char *, const char *);
};

#endif
